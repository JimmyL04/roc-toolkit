/*
 * Copyright (c) 2021 Roc Streaming authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "roc_audio/sample_spec.h"
#include "roc_core/panic.h"
#include "roc_packet/units.h"

namespace roc {
namespace audio {

SampleSpec::SampleSpec() {
    set_sample_rate(0);
    set_channel_mask(0);
}

SampleSpec::SampleSpec(size_t sample_rate, packet::channel_mask_t channel_mask) {
    roc_panic_if(sample_rate == 0);
    roc_panic_if(channel_mask == 0);
    set_sample_rate(sample_rate);
    set_channel_mask(channel_mask);
}

size_t SampleSpec::sample_rate() const {
    return sample_rate_;
}

void SampleSpec::set_sample_rate(size_t sample_rate) {
    sample_rate_ = sample_rate;
}

packet::channel_mask_t SampleSpec::channel_mask() const {
    return channel_mask_;
}

size_t SampleSpec::num_channels() const {
    return num_channels_;
}

void SampleSpec::set_channel_mask(packet::channel_mask_t channel_mask) {
    channel_mask_ = channel_mask;
    num_channels_ = packet::num_channels(channel_mask);
}

packet::timestamp_diff_t SampleSpec::timestamp_from_ns(core::nanoseconds_t ns) const {
    return packet::timestamp_diff_t(roundf(float(ns) / core::Second * sample_rate_));
}

core::nanoseconds_t SampleSpec::timestamp_to_ns(packet::timestamp_diff_t ts) const {
    return core::nanoseconds_t(roundf(float(ts) / sample_rate_ * core::Second));
}

size_t SampleSpec::ns_to_soa(core::nanoseconds_t frame_length) const {
    return (size_t)timestamp_from_ns(frame_length) * num_channels();
}

core::nanoseconds_t SampleSpec::soa_to_ns(size_t frame_length) const {
    return timestamp_to_ns(packet::timestamp_diff_t(frame_length / num_channels()));
}

} // namespace audio
} // namespace roc
