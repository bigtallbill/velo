#include "core/Document.h"
#include "engine/AudioEngine.h"
#include "engine/Compositor.h"
#include "engine/Exporter.h"
#include "ui/MainWindow.h"
#include "ui/Theme.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QEventLoop>
#include <QIcon>
#include <QProcess>
#include <cstdio>

// Headless engine smoke test: generates media with ffmpeg, builds a project,
// renders a frame and exports a short file. Run with `velo --selftest`.
static int selftest() {
    const QString dir = QDir::tempPath() + "/velo_selftest";
    QDir().mkpath(dir);
    const QString vid = dir + "/test.mp4";
    QProcess gen;
    gen.start("ffmpeg",
              {"-y", "-f", "lavfi", "-i", "testsrc2=size=640x360:rate=30",
               "-f", "lavfi", "-i", "sine=frequency=440", "-t", "4", "-c:v",
               "libx264", "-pix_fmt", "yuv420p", "-c:a", "aac", vid});
    gen.waitForFinished(30000);
    if (gen.exitCode() != 0) {
        fprintf(stderr, "selftest: failed to generate test media\n");
        return 1;
    }

    Document doc;
    const QStringList ids = doc.importMedia({vid});
    if (ids.size() != 1) {
        fprintf(stderr, "selftest: import failed\n");
        return 1;
    }
    const QString seqId = doc.sequenceFromMedia(ids.first());
    Sequence *seq = doc.project().sequenceById(seqId);
    if (!seq || seq->duration() < 3.0) {
        fprintf(stderr, "selftest: sequence creation failed\n");
        return 1;
    }
    // exercise editing ops
    doc.splitAt(seqId, 2.0, false);
    doc.addTextClip(seqId, 2, 0.5);
    {
        // close-gap regression: move the 2nd A/V pair right, close the gap
        Sequence *sq = doc.project().sequenceById(seqId);
        Clip *v1 = &sq->videoTracks[0].clips[1];
        const double oldStart = v1->start;
        for (Clip *c : {&sq->videoTracks[0].clips[1], &sq->audioTracks[0].clips[1]})
            c->start += 5.0;
        doc.closeGap(seqId, TrackType::Video, 0, oldStart + 2.5);
        sq = doc.project().sequenceById(seqId);
        const double vs = sq->videoTracks[0].clips[1].start;
        const double as = sq->audioTracks[0].clips[1].start;
        if (std::abs(vs - oldStart) > 1e-6 || std::abs(as - oldStart) > 1e-6) {
            fprintf(stderr, "selftest: closeGap failed (v=%.3f a=%.3f want %.3f)\n",
                    vs, as, oldStart);
            return 1;
        }
    }

    Compositor comp(&doc.project(), doc.mutex());
    QImage frame = comp.renderFrame(seqId, 1.0, 0.5);
    if (frame.isNull() || frame.width() != 320) {
        fprintf(stderr, "selftest: render failed (%dx%d)\n", frame.width(),
                frame.height());
        return 1;
    }
    AudioMixer mixer(&doc.project(), doc.mutex());
    QVector<float> buf(4800 * 2);
    mixer.mix(seqId, 1.0, 4800, buf.data());
    float peak = 0;
    for (float v : buf) peak = qMax(peak, std::abs(v));
    if (peak < 0.01f) {
        fprintf(stderr, "selftest: audio mix silent\n");
        return 1;
    }

    // save & reload round-trip
    const QString proj = dir + "/test.velo";
    if (!doc.saveProject(proj) || !doc.loadProject(proj)) {
        fprintf(stderr, "selftest: project save/load failed\n");
        return 1;
    }

    ExportSettings es;
    es.outputPath = dir + "/out.mp4";
    es.width = 640;
    es.height = 360;
    es.fps = 30;
    es.crf = 28;
    Exporter exp(&doc.project(), doc.mutex(), seqId, es);
    bool ok = false;
    QString msg;
    QEventLoop loop;
    QObject::connect(&exp, &Exporter::finished, &loop,
                     [&](bool o, const QString &m) {
                         ok = o;
                         msg = m;
                         loop.quit();
                     });
    exp.start();
    loop.exec();
    exp.wait(10000);
    if (!ok) {
        fprintf(stderr, "selftest: export failed: %s\n", qPrintable(msg));
        return 1;
    }
    printf("selftest: OK (render, mix, save/load, export -> %s)\n",
           qPrintable(es.outputPath));
    return 0;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Velo");
    app.setOrganizationName("velo");
    app.setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({"selftest", "Run a headless engine self test and exit."});
    parser.addPositionalArgument("project", "Project file (.velo) to open.");
    parser.process(app);

    if (parser.isSet("selftest")) return selftest();

    Theme::apply(app);
    app.setWindowIcon(QIcon(QStringLiteral(":/velo.svg")));
    MainWindow win;
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) win.document()->loadProject(args.first());
    win.show();
    return app.exec();
}
