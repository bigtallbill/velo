#pragma once
#include "core/Model.h"
#include "media/MediaCache.h"
#include <QAudioSink>
#include <QIODevice>
#include <QObject>
#include <QRecursiveMutex>
#include <memory>

// Sample-accurate mixer for a sequence (handles nesting, speed, keyframed
// clip/track volume and audio effects). Not thread safe by itself; guarded
// by the model mutex during mix().
class AudioMixer {
public:
    AudioMixer(Project *project, QRecursiveMutex *mutex)
        : m_project(project), m_mutex(mutex) {}
    static constexpr int kRate = AudioReader::kRate;
    // Mix nFrames stereo float frames of the sequence starting at time t.
    void mix(const QString &seqId, double t, int nFrames, float *out);
    void dropReaders() { m_readers.clear(); }

private:
    void mixSequence(const Sequence &seq, double t, int nFrames, float *out,
                     int depth);
    AudioReader *readerFor(const Clip &clip, const QString &path);
    Project *m_project;
    QRecursiveMutex *m_mutex;
    QHash<quint64, std::shared_ptr<AudioReader>> m_readers;
};

// ---------------------------------------------------------------------------
class AudioEngine : public QObject {
    Q_OBJECT
public:
    AudioEngine(Project *project, QRecursiveMutex *mutex, QObject *parent = nullptr);
    ~AudioEngine() override;

    void play(const QString &seqId, double t);
    void stop();
    bool isPlaying() const { return m_playing; }
    double clock() const;  // current playback time in sequence seconds
    void invalidateReaders();

private:
    friend class MixDevice;
    AudioMixer m_mixer;
    std::unique_ptr<QAudioSink> m_sink;
    QIODevice *m_device = nullptr;  // owned by this (MixDevice)
    QString m_seqId;
    double m_startT = 0;
    bool m_playing = false;
};
