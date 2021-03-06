/*
 * nvbio
 * Copyright (c) 2011-2014, NVIDIA CORPORATION. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the NVIDIA CORPORATION nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <nvbio/io/sequence/sequence.h>
#include <stdio.h>
#include <stdlib.h>

namespace nvbio {
namespace io {

///@addtogroup IO
///@{

///@addtogroup SequenceIO
///@{

///@addtogroup SequenceIODetail
///@{

/// abstract file-backed SequenceDataStream
///
struct SequenceDataFile : public SequenceDataStream
{
    struct Options
    {
        Options() :
            qualities(Phred),
            max_seqs(uint32(-1)), 
            max_sequence_len(uint32(-1)),
            trim3(0),
            trim5(0),
            flags(FORWARD) {}

        QualityEncoding    qualities;
        uint32             max_seqs;
        uint32             max_sequence_len;
        uint32             trim3;
        uint32             trim5;
        SequenceEncoding   flags;
    };

    static const uint32 LONG_READ = 32*1024;

    /// enum describing various possible file states
    ///
    typedef enum {
        // not yet opened (initial state)
        FILE_NOT_READY,
        // unable to open file (e.g., file not found)
        FILE_OPEN_FAILED,
        // ready to read
        FILE_OK,
        // reached EOF
        FILE_EOF,
        // file stream error (e.g., gzip CRC failure)
        FILE_STREAM_ERROR,
        // file format parsing error (e.g., bad FASTQ file)
        FILE_PARSE_ERROR
    } FileState;

protected:
    SequenceDataFile(
        const Options&          options)
      : SequenceDataStream(),
        m_options( options ),
        m_loaded(0),
        m_file_state(FILE_NOT_READY)
    {};

public:
    /// virtual destructor
    ///
    virtual ~SequenceDataFile() {}

    /// grab the next batch of reads into a host memory buffer
    ///
    virtual int next(struct SequenceDataEncoder* encoder, const uint32 batch_size, const uint32 batch_bps);

    /// returns true if the stream is ready to read from
    ///
    virtual bool is_ok(void)
    {
        return m_file_state == FILE_OK;
    };

protected:
    virtual int nextChunk(struct SequenceDataEncoder* encoder, uint32 max_reads, uint32 max_bps) = 0;

    Options                 m_options;
    uint32                  m_loaded;

    // current file state
    FileState               m_file_state;
};

///@} // SequenceIODetail
///@} // SequenceIO
///@} // IO

} // namespace io
} // namespace nvbio
