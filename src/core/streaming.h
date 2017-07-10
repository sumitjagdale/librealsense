// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.
#pragma once

#include "options.h"
#include "types.h"

#include <functional>

namespace librealsense
{
    class stream_profile_interface
    {
    public:
        virtual bool  is_recommended() const = 0;
        virtual size_t get_size() const = 0;
        virtual unsigned int get_fps() const = 0;

        virtual ~stream_profile_interface() = default;
    };

    class sensor_interface;
    class archive_interface;

    class frame_interface
    {
    public:
        virtual rs2_metadata_t get_frame_metadata(const rs2_frame_metadata& frame_metadata) const = 0;
        virtual bool supports_frame_metadata(const rs2_frame_metadata& frame_metadata) const = 0;
        virtual const byte* get_frame_data() const = 0;
        virtual rs2_time_t get_frame_timestamp() const = 0;
        virtual rs2_timestamp_domain get_frame_timestamp_domain() const = 0;
        virtual void set_timestamp(double new_ts) = 0;
        virtual unsigned long long get_frame_number() const = 0;

        virtual void set_timestamp_domain(rs2_timestamp_domain timestamp_domain) = 0;

        virtual rs2_time_t get_frame_system_time() const = 0;
        virtual rs2_format get_format() const = 0;
        virtual rs2_stream get_stream_type() const = 0;
        virtual int get_framerate() const = 0;

        virtual rs2_time_t get_frame_callback_start_time_point() const = 0;
        virtual void update_frame_callback_start_ts(rs2_time_t ts) = 0;

        virtual void acquire() = 0;
        virtual void release() = 0;
        virtual frame_interface* publish(std::shared_ptr<archive_interface> new_owner) = 0;
        virtual void attach_continuation(frame_continuation&& continuation) = 0;
        virtual void disable_continuation() = 0;

        virtual archive_interface* get_owner() const = 0;

        virtual ~frame_interface() = default;
    };

    using on_frame = std::function<void(frame_interface*)>;
    using stream_profiles = std::vector<std::shared_ptr<stream_profile_interface>>;

    class info_interface
    {
    public:
        virtual const std::string& get_info(rs2_camera_info info) const = 0;
        virtual bool supports_info(rs2_camera_info info) const = 0;

        virtual ~info_interface() = default;
    };

    class sensor_interface : public virtual info_interface, public virtual options_interface
    {
    public:
        virtual std::vector<stream_profile> get_principal_requests() = 0;
        virtual void open(const std::vector<stream_profile>& requests) = 0;
        virtual void close() = 0;

        virtual void register_notifications_callback(notifications_callback_ptr callback) = 0;

        virtual void start(frame_callback_ptr callback) = 0;
        virtual void stop() = 0;

        virtual bool is_streaming() const = 0;

//        virtual rs2_stream   get_stream_type(unsigned int index) const = 0;
//        virtual const char*  get_stream_desciption(unsigned int index) const = 0;
//        virtual unsigned int get_streams_count() const = 0;

//        virtual stream_profiles get_profiles() const = 0;

//        virtual void start(const stream_profile_interface& profile) = 0;

//        virtual void stop() = 0;

//        virtual void set_callback(on_frame callback) = 0;

//        virtual bool is_streaming() const = 0;

        virtual ~sensor_interface() = default;
    };

    class device_interface : public virtual info_interface
    {
    public:
        virtual sensor_interface& get_sensor(size_t i) = 0;
        virtual const sensor_interface& get_sensor(size_t i) const = 0;
        virtual size_t get_sensors_count() const = 0;

        virtual void hardware_reset() = 0;

        virtual rs2_extrinsics get_extrinsics(size_t from, rs2_stream from_stream, size_t to, rs2_stream to_stream) const = 0;

        virtual ~device_interface() = default;
    };
}
