extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswresample/swresample.h>
}

#include <iostream>
#include <fstream>
#include <atomic>

class Decoder{
protected:

    std::atomic<bool> mp3_counter;

    // Resource

    AVFormatContext* fmt = nullptr;

    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    AVCodecContext* ctx = nullptr;
    AVChannelLayout outLayout;

    SwrContext* swr = nullptr;

    float volume = 0.2f;

    std::string filename;

public:

    Decoder(const std::string &path) : filename(path) {
        avformat_network_init();
        mp3_counter.store(true,std::memory_order_relaxed);
    }

    void mp3_decode() {

        if (avformat_open_input(&fmt, filename.c_str(), nullptr, nullptr) < 0) {
            std::cerr << "ERROR : failed to open file" << std::endl;
            throw std::runtime_error("ERROR : failed to open file");
        }

        avformat_find_stream_info(fmt, nullptr);

        int audioStream = -1;
        for (int i = 0; i < fmt->nb_streams; i++) {
            if (fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audioStream = i;
                break;
            }
        }
        if (audioStream == -1) return;

        AVCodecParameters* cp = fmt->streams[audioStream]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(cp->codec_id);
        this->ctx = avcodec_alloc_context3(codec);

        avcodec_parameters_to_context(ctx, cp);
        avcodec_open2(ctx, codec, nullptr);

        av_channel_layout_default(&outLayout, 2);
        swr_alloc_set_opts2(
            &swr,
            &outLayout,
            AV_SAMPLE_FMT_S16,
            44100,
            &ctx->ch_layout,
            ctx->sample_fmt,
            ctx->sample_rate,
            0,
            nullptr
        );
        swr_init(swr);

        this->pkt = av_packet_alloc();
        this->frame = av_frame_alloc();

        std::cout << "INFO : MP3 Decode" << std::endl;

        while (av_read_frame(fmt, pkt) == 0 && this->mp3_counter.load(std::memory_order_relaxed)) {
            if (pkt->stream_index == audioStream) {
                avcodec_send_packet(ctx, pkt);

                while (avcodec_receive_frame(ctx, frame) == 0) {
                    uint8_t* out[1];
                    int outSamples = swr_get_out_samples(swr, frame->nb_samples);

                    av_samples_alloc(out, nullptr, 2, outSamples, AV_SAMPLE_FMT_S16, 0);

                    int converted = swr_convert(
                        swr,
                        out, outSamples,
                        const_cast<const uint8_t**>(frame->data),
                        frame->nb_samples
                    );

                    short* samples = (short*)out[0];
                    int totalSamples = converted * 2;

                    for (int i = 0; i < totalSamples; i++) {
                        float v = samples[i] * volume;
                        if (v > 32767) v = 32767;
                        if (v < -32768) v = -32768;
                        samples[i] = (short)v;
                    }

                    int dataSize = converted * 2 * 2;

                    fwrite(out[0], 1, dataSize, stdout);

                    av_freep(&out[0]);
                }
            }
            av_packet_unref(pkt);
        }
    }

    ~Decoder(){

        av_frame_free(&frame);
        av_packet_free(&pkt);
        swr_free(&swr);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt);
    }
};