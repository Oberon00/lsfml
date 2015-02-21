// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "open_util.hpp"

#include <lsfml/resource_sound.hpp>
#include <lsfml/vector.hpp>

#include <apollo/create_class.hpp>
#include <apollo/operator.hpp>
#include <apollo/property.hpp>

#include <SFML/Audio.hpp>

namespace {

int sound_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<lsfml::resource_sound>(L);
    } else if (n_args == 1) {
        apollo::emplace_object<lsfml::resource_sound>(L,
            APOLLO_TO_ARG(L, 1, lsfml::resource_sound::res_ptr));
    } else {
        throw std::invalid_argument("Bad argument count for Sound.new");
    }
    return 1;
}

int sound_set_position(lua_State* L)
{
    auto& self = apollo::to<sf::Sound&>(L, 1);
    int const n_args = lua_gettop(L);
    if (n_args == 2) {
        self.setPosition(APOLLO_TO_ARG(L, 2, sf::Vector3f const&));
    } else if (n_args == 4) {
        self.setPosition(
            apollo::to<float>(L, 2),
            apollo::to<float>(L, 3),
            apollo::to<float>(L, 4));
    } else {
        throw std::invalid_argument("Bad argument count for Sound.set_position");
    }
    return 0;
}

int sound_recorder_start(lua_State* L)
{
    auto& self = apollo::to<sf::SoundRecorder&>(L, 1);
    return apollo::push(L, self.start(apollo::to(L, 2, 44100u)));
}

std::shared_ptr<sf::SoundBuffer> sound_buffer_recorder_buffer(
    sf::SoundBufferRecorder const& self)
{
    return std::make_shared<sf::SoundBuffer>(self.getBuffer());
}

int sound_recorder_available_devices(lua_State* L)
{
    auto avail = sf::SoundRecorder::getAvailableDevices();
    lua_createtable(L, avail.size(), 0);
    for (std::size_t i = 0; i < avail.size(); ++i) {
        apollo::push(L, avail[i]);
        lua_rawseti(L, -2, i);
    }
    return 1;
}

template <typename T>
void add_sound_members(lua_State* L, int idx)
{
    apollo::rawset_table(L, idx)
        ("play", FN(lsfml::resource_sound::play))
        ("pause", FN(lsfml::resource_sound::pause))
        ("stop", FN(lsfml::resource_sound::stop))
        ("set_loop", FN(lsfml::resource_sound::setLoop))
        ("loop", FN(lsfml::resource_sound::getLoop))
        ("set_playing_offset", FN(lsfml::resource_sound::setPlayingOffset))
        ("playing_offset", FN(lsfml::resource_sound::getPlayingOffset))
        ("status", FN(lsfml::resource_sound::getStatus));
}

template <
    void(*Xyzfn)(float, float, float),
    void(*Vec3fn)(sf::Vector3f const&)>
int vec3f_fn_impl(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 1) {
        Vec3fn(APOLLO_TO_ARG(L, 1, sf::Vector3f const&));
    } else if (n_args == 3) {
        Xyzfn(
            apollo::to<float>(L, 1),
            apollo::to<float>(L, 2),
            apollo::to<float>(L, 3));
    } else {
        throw std::invalid_argument(
            "Bad argument count! Expected three scalars or single vector.");
    }
    return 0;
}

template <
    void(*Xyzfn)(float, float, float),
    void(*Vec3fn)(sf::Vector3f const&)>
apollo::raw_function vec3f_fn()
{
    return apollo::raw_function::caught<&vec3f_fn_impl<Xyzfn, Vec3fn>>();
}


} // anonymous namespace

LSFML_MODULE(lsfml_audio)
{
    // SoundBuffer //
    apollo::new_table(L)
        ("new", FN(apollo::new_wrapper<std::shared_ptr<sf::SoundBuffer>>))
        ("load_from_file", FN(sf::SoundBuffer::loadFromFile))
        ("save_to_file", FN(sf::SoundBuffer::saveToFile))
        ("sample_count", FN(sf::SoundBuffer::getSampleCount))
        ("sample_rate", FN(sf::SoundBuffer::getSampleRate))
        ("channel_count", FN(sf::SoundBuffer::getChannelCount))
        ("duration", FN(sf::SoundBuffer::getDuration));
        // Missing: getSamples, loadFromSamples
    apollo::stack_reference soundbuf_members(L, -1);
    apollo::export_class<sf::SoundBuffer>(L)("__index", soundbuf_members);


    // SoundSource //
    apollo::new_table(L)
        ("set_pitch", FN(sf::SoundSource::setPitch))
        ("pitch", FN(sf::SoundSource::getPitch))
        ("set_volume", FN(sf::SoundSource::setVolume))
        ("volume", FN(sf::SoundSource::getVolume))
        ("set_position", apollo::raw_function::caught<&sound_set_position>())
        ("position", FN(sf::SoundSource::getPosition))
        ("set_relative_to_listener",
            FN(sf::SoundSource::setRelativeToListener))
        ("is_relative_to_listener",
            FN(sf::SoundSource::isRelativeToListener))
        ("set_min_distance", FN(sf::SoundSource::setMinDistance))
        ("min_distance", FN(sf::SoundSource::getMinDistance))
        ("set_attentuation", FN(sf::SoundSource::setAttenuation))
        ("attentuation", FN(sf::SoundSource::getAttenuation))
        ("STOPPED", sf::SoundSource::Stopped)
        ("PAUSED", sf::SoundSource::Paused)
        ("PLAYING", sf::SoundSource::Playing);
    apollo::stack_reference soundsrc_members(L, -1);
    apollo::export_class<sf::SoundSource>(L)
        ("__index", soundsrc_members);

    // Sound //
    lua_newtable(L);
    apollo::stack_reference sound_members(L, -1);
    apollo::extend_table(L, sound_members.get(), soundsrc_members.get());
    apollo::rawset_table(L, sound_members.get())
        ("new", apollo::raw_function::caught<&sound_new>())
        ("set_buffer", FN(lsfml::resource_sound::set_resource))
        ("buffer", FN(lsfml::resource_sound::resource));
        // Left out: resetBuffer (SFML internal)
    add_sound_members<lsfml::resource_sound>(L, sound_members.get());
    apollo::register_class<sf::Sound, sf::SoundSource>(L);
    apollo::export_class<lsfml::resource_sound, sf::Sound>(L)
        ("__index", sound_members);

    // SoundStream //
    lua_newtable(L);
    apollo::stack_reference soundstream_members(L, -1);
    apollo::extend_table(L, soundstream_members.get(), soundsrc_members.get());
    apollo::rawset_table(L, soundstream_members.get())
        ("channel_count", FN(sf::SoundStream::getChannelCount))
        ("sample_rate", FN(sf::SoundStream::getSampleRate));
    add_sound_members<sf::SoundStream>(L, soundstream_members.get());
    apollo::export_class<sf::SoundStream, sf::SoundSource>(L)
        ("__index", soundstream_members);

    // Music //
    lua_newtable(L);
    apollo::stack_reference music_members(L, -1);
    apollo::extend_table(L, music_members.get(), soundstream_members.get());
    apollo::rawset_table(L, music_members.get())
        ("new", apollo::get_raw_emplace_ctor_wrapper<sf::Music>())
        ("open_from_file", FN(sf::Music::openFromFile))
        ("duration", FN(sf::Music::getDuration));
    apollo::export_class<sf::Music, sf::SoundStream>(L)
        ("__index", music_members);

    // SoundRecorder //
    apollo::new_table(L)
        ("start", apollo::raw_function::caught<&sound_recorder_start>())
        ("stop", FN(sf::SoundRecorder::stop))
        ("sample_rate", FN(sf::SoundRecorder::getSampleRate))
        ("set_device", FN(sf::SoundRecorder::setDevice))
        ("device", FN(sf::SoundRecorder::getDevice))
        ("default_device", FN(sf::SoundRecorder::getDefaultDevice))
        ("is_available", FN(sf::SoundRecorder::isAvailable))
        ("available_devices",
            apollo::raw_function::caught<&sound_recorder_available_devices>());
    apollo::stack_reference soundrec_members(L, -1);
    apollo::export_class<sf::SoundRecorder>(L)
        ("__index", soundrec_members);

    // SoundBufferRecorder //
    lua_newtable(L);
    apollo::stack_reference soundbufrec_members(L, -1);
    apollo::extend_table(L, soundbufrec_members.get(), soundrec_members.get());
    apollo::rawset_table(L, soundbufrec_members.get())
        ("new",
            apollo::get_raw_emplace_ctor_wrapper<sf::SoundBufferRecorder>())
        ("buffer", FN(sound_buffer_recorder_buffer));
    apollo::export_class<sf::SoundBufferRecorder, sf::SoundRecorder>(L)
        ("__index", soundbufrec_members);

    apollo::new_table(L) // Audio module
        ("SoundBuffer", soundbuf_members)
        ("SoundSource", soundsrc_members)
        ("Sound", sound_members)
        ("SoundStream", soundstream_members)
        ("Music", music_members)
        ("SoundRecorder", soundrec_members)
        ("SoundBufferRecorder", soundbufrec_members)
        .subtable("listener")
            ("set_global_volume", FN(sf::Listener::setGlobalVolume))
            ("global_volume", FN(sf::Listener::getGlobalVolume))
            ("set_position", vec3f_fn<
                &sf::Listener::setPosition, &sf::Listener::setPosition>())
            ("position", FN(sf::Listener::getPosition))
            ("set_direction", vec3f_fn<
                &sf::Listener::setDirection, &sf::Listener::setDirection>())
            ("direction", FN(sf::Listener::getDirection))
            ("set_up_vector", vec3f_fn<
                &sf::Listener::setUpVector, &sf::Listener::setUpVector>())
            ("up_vector", FN(sf::Listener::getUpVector))
        .end_subtable();
    return 1;
}
