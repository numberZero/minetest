// This file is automatically generated

#pragma once
#include <atomic>
#include "settings_static.h"

inline namespace setting_type {

enum class JoystickType {
	Auto,
	Generic,
	Xbox,
};

enum class LeavesStyle {
	Fancy,
	Simple,
	Opaque,
};

enum class NodeHighlighting {
	Box,
	Halo,
	None,
};

enum class VideoDriver {
	Null,
	Software,
	Burningsvideo,
	Direct3d8,
	Direct3d9,
	Opengl,
};

enum class _3dMode {
	None,
	Anaglyph,
	Interlaced,
	Topbottom,
	Sidebyside,
	Pageflip,
};

enum class WorldAlignedMode {
	Disable,
	Enable,
	ForceSolid,
	ForceNodebox,
};

enum class AutoscaleMode {
	Disable,
	Enable,
	Force,
};

enum class ScreenshotFormat {
	Png,
	Jpg,
	Bmp,
	Pcx,
	Ppm,
	Tga,
};

enum class DeprecatedLuaApiHandling {
	Legacy,
	Log,
	Error,
};

enum class ProfilerDefaultReportFormat {
	Txt,
	Csv,
	Lua,
	Json,
	JsonPretty,
};

enum class Language {
	_,
	Be,
	Ca,
	Cs,
	Da,
	De,
	En,
	Eo,
	Es,
	Et,
	Fr,
	He,
	Hu,
	Id,
	It,
	Ja,
	Jbo,
	Ko,
	Ky,
	Lt,
	Nb,
	Nl,
	Pl,
	Pt,
	PtBR,
	Ro,
	Ru,
	SrCyrl,
	Tr,
	Uk,
	ZhCN,
	ZhTW,
};

enum class DebugLogLevel {
	_,
	None,
	Error,
	Warning,
	Action,
	Info,
	Verbose,
};

enum class MgName {
	V5,
	V6,
	V7,
	Flat,
	Valleys,
	Fractal,
	Singlenode,
};

}

struct BuiltinSettings {

/*
 * Controls
 */
	std::atomic<bool> enable_build_where_you_stand{false};
	std::atomic<bool> free_move{false};
	std::atomic<bool> fast_move{false};
	std::atomic<bool> noclip{false};
	std::atomic<bool> cinematic{false};
	std::atomic<float> camera_smoothing{0.0};
	std::atomic<float> cinematic_camera_smoothing{0.7};
	std::atomic<bool> invert_mouse{false};
	std::atomic<float> mouse_sensitivity{0.2};
	std::atomic<bool> aux1_descends{false};
	std::atomic<bool> doubletap_jump{false};
	std::atomic<bool> always_fly_fast{true};
	std::atomic<float> repeat_rightclick_time{0.25};
	std::atomic<bool> safe_dig_and_place{false};
	std::atomic<bool> random_input{false};
	std::atomic<bool> continuous_forward{false};
	std::atomic<bool> enable_joysticks{false};
	std::atomic<int> joystick_id{0};
	std::atomic<JoystickType> joystick_type{JoystickType::Auto};
	std::atomic<float> repeat_joystick_button_time{0.17};
	std::atomic<float> joystick_frustum_sensitivity{170};

/*
 * Graphics
 */

/* In-Game */

/** Basic */
	std::atomic<bool> enable_vbo{true};
	std::atomic<bool> enable_fog{true};
	std::atomic<LeavesStyle> leaves_style{LeavesStyle::Fancy};
	std::atomic<bool> connected_glass{false};
	std::atomic<bool> smooth_lighting{true};
	std::atomic<bool> enable_clouds{true};
	std::atomic<bool> enable_3d_clouds{true};
	std::atomic<NodeHighlighting> node_highlighting{NodeHighlighting::Box};
	std::atomic<bool> enable_particles{true};

/** Filtering */
	std::atomic<bool> mip_map{false};
	std::atomic<bool> anisotropic_filter{false};
	std::atomic<bool> bilinear_filter{false};
	std::atomic<bool> trilinear_filter{false};
	std::atomic<bool> texture_clean_transparent{false};
	std::atomic<int> texture_min_size{64};
	std::atomic<int> fsaa{0};
	std::atomic<int> undersampling{0};

/** Shaders */
	std::atomic<bool> enable_shaders{true};

/*** Tone Mapping */
	std::atomic<bool> tone_mapping{false};

/*** Bumpmapping */
	std::atomic<bool> enable_bumpmapping{false};
	std::atomic<bool> generate_normalmaps{false};
	std::atomic<float> normalmaps_strength{0.6};
	std::atomic<int> normalmaps_smooth{0};

/*** Parallax Occlusion */
	std::atomic<bool> enable_parallax_occlusion{false};
	std::atomic<int> parallax_occlusion_mode{1};
	std::atomic<float> _3d_paralax_strength{0.025};
	std::atomic<int> parallax_occlusion_iterations{4};
	std::atomic<float> parallax_occlusion_scale{0.08};
	std::atomic<float> parallax_occlusion_bias{0.04};

/*** Waving Nodes */
	std::atomic<bool> enable_waving_water{false};
	std::atomic<float> water_wave_height{1.0};
	std::atomic<float> water_wave_length{20.0};
	std::atomic<float> water_wave_speed{5.0};
	std::atomic<bool> enable_waving_leaves{false};
	std::atomic<bool> enable_waving_plants{false};

/** Advanced */
	std::atomic<bool> arm_inertia{true};
	std::atomic<int> fps_max{60};
	std::atomic<int> pause_fps_max{20};
	std::atomic<int> viewing_range{100};
	std::atomic<float> near_plane{0.1};
	std::atomic<int> screen_w{1024};
	std::atomic<int> screen_h{600};
	std::atomic<bool> autosave_screensize{true};
	std::atomic<bool> fullscreen{false};
	std::atomic<int> fullscreen_bpp{24};
	std::atomic<bool> vsync{false};
	std::atomic<int> fov{72};
	std::atomic<int> zoom_fov{15};
	std::atomic<float> display_gamma{1.0};
	std::atomic<float> lighting_alpha{0.0};
	std::atomic<float> lighting_beta{1.5};
	std::atomic<float> lighting_boost{0.2};
	std::atomic<float> lighting_boost_center{0.5};
	std::atomic<float> lighting_boost_spread{0.2};
	std::atomic<VideoDriver> video_driver{VideoDriver::Opengl};
	std::atomic<int> cloud_radius{12};
	std::atomic<float> view_bobbing_amount{1.0};
	std::atomic<float> fall_bobbing_amount{0.0};
	std::atomic<_3dMode> _3d_mode{_3dMode::None};
	std::atomic<float> console_height{1.0};
	std::atomic<int> console_alpha{200};
	std::atomic<int> formspec_fullscreen_bg_opacity{140};
	std::atomic<int> formspec_default_bg_opacity{140};
	std::atomic<int> selectionbox_width{2};
	std::atomic<int> crosshair_alpha{255};
	std::atomic<int> recent_chat_messages{6};
	std::atomic<bool> desynchronize_mapblock_texture_animation{true};
	std::atomic<float> hud_hotbar_max_width{1.0};
	std::atomic<float> hud_scaling{1.0};
	std::atomic<bool> enable_mesh_cache{false};
	std::atomic<int> mesh_generation_interval{0};
	std::atomic<int> meshgen_block_cache_size{20};
	std::atomic<bool> enable_minimap{true};
	std::atomic<bool> minimap_shape_round{true};
	std::atomic<bool> minimap_double_scan_height{true};
	std::atomic<bool> directional_colored_fog{true};
	std::atomic<float> ambient_occlusion_gamma{2.2};
	std::atomic<bool> inventory_items_animations{false};
	std::atomic<bool> inventory_image_hack{false};
	std::atomic<float> fog_start{0.4};
	std::atomic<bool> opaque_water{false};
	std::atomic<WorldAlignedMode> world_aligned_mode{WorldAlignedMode::Enable};
	std::atomic<AutoscaleMode> autoscale_mode{AutoscaleMode::Disable};
	std::atomic<bool> show_entity_selectionbox{true};

/* Menus */
	std::atomic<bool> menu_clouds{true};
	std::atomic<float> gui_scaling{1.0};
	std::atomic<bool> gui_scaling_filter{false};
	std::atomic<bool> gui_scaling_filter_txr2img{true};
	std::atomic<int> tooltip_show_delay{400};
	std::atomic<bool> tooltip_append_itemname{false};
	std::atomic<bool> freetype{true};
	std::atomic<int> font_size{16};
	std::atomic<int> font_shadow{1};
	std::atomic<int> font_shadow_alpha{127};
	std::atomic<int> mono_font_size{15};
	std::atomic<int> fallback_font_size{15};
	std::atomic<int> fallback_font_shadow{1};
	std::atomic<int> fallback_font_shadow_alpha{128};
	std::atomic<ScreenshotFormat> screenshot_format{ScreenshotFormat::Png};
	std::atomic<int> screenshot_quality{0};

/* Advanced */
	std::atomic<int> screen_dpi{72};
	std::atomic<bool> enable_console{false};

/*
 * Sound
 */
	std::atomic<bool> enable_sound{true};
	std::atomic<float> sound_volume{0.7};
	std::atomic<bool> mute_sound{false};

/*
 * Client
 */

/* Network */
	std::atomic<int> remote_port{30000};
	std::atomic<bool> enable_local_map_saving{false};
	std::atomic<bool> enable_remote_media_server{true};
	std::atomic<bool> enable_client_modding{false};
	std::atomic<int> max_out_chat_queue_size{20};

/* Advanced */
	std::atomic<int> client_unload_unused_data_timeout{600};
	std::atomic<int> client_mapblock_limit{5000};
	std::atomic<bool> show_debug{false};

/*
 * Server / Singleplayer
 */
	std::atomic<bool> server_announce{false};
	std::atomic<bool> strip_color_codes{false};

/* Network */
	std::atomic<int> port{30000};
	std::atomic<bool> strict_protocol_version_checking{false};
	std::atomic<bool> ipv6_server{false};

/** Advanced */
	std::atomic<int> max_simultaneous_block_sends_per_client{10};
	std::atomic<float> full_block_send_enable_min_time_from_building{2.0};
	std::atomic<int> max_packets_per_iteration{1024};

/* Game */
	std::atomic<int> max_users{15};
	std::atomic<int> item_entity_ttl{900};
	std::atomic<bool> show_statusline_on_connect{true};
	std::atomic<bool> enable_damage{false};
	std::atomic<bool> creative_mode{false};
	std::atomic<bool> unlimited_player_transfer_distance{true};
	std::atomic<int> player_transfer_distance{0};
	std::atomic<bool> enable_pvp{true};
	std::atomic<bool> enable_mod_channels{false};
	std::atomic<bool> disallow_empty_password{false};
	std::atomic<bool> disable_anticheat{false};
	std::atomic<bool> enable_rollback_recording{false};
	std::atomic<bool> ask_reconnect_on_crash{false};
	std::atomic<int> active_object_send_range_blocks{3};
	std::atomic<int> active_block_range{3};
	std::atomic<int> max_block_send_distance{10};
	std::atomic<int> max_forceloaded_blocks{16};
	std::atomic<int> time_send_interval{5};
	std::atomic<int> time_speed{72};
	std::atomic<int> world_start_time{5250};
	std::atomic<float> server_map_save_interval{5.3};

/** Physics */
	std::atomic<float> movement_acceleration_default{3};
	std::atomic<float> movement_acceleration_air{2};
	std::atomic<float> movement_acceleration_fast{10};
	std::atomic<float> movement_speed_walk{4};
	std::atomic<float> movement_speed_crouch{1.35};
	std::atomic<float> movement_speed_fast{20};
	std::atomic<float> movement_speed_climb{3};
	std::atomic<float> movement_speed_jump{6.5};
	std::atomic<float> movement_liquid_fluidity{1};
	std::atomic<float> movement_liquid_fluidity_smooth{0.5};
	std::atomic<float> movement_liquid_sink{10};
	std::atomic<float> movement_gravity{9.81};

/** Advanced */
	std::atomic<DeprecatedLuaApiHandling> deprecated_lua_api_handling{DeprecatedLuaApiHandling::Legacy};
	std::atomic<int> max_clearobjects_extra_loaded_blocks{4096};
	std::atomic<int> server_unload_unused_data_timeout{29};
	std::atomic<int> max_objects_per_block{64};
	std::atomic<int> sqlite_synchronous{2};
	std::atomic<float> dedicated_server_step{0.09};
	std::atomic<float> active_block_mgmt_interval{2.0};
	std::atomic<float> abm_interval{1.0};
	std::atomic<float> nodetimer_interval{0.2};
	std::atomic<bool> ignore_world_load_errors{false};
	std::atomic<int> liquid_loop_max{100000};
	std::atomic<int> liquid_queue_purge_time{0};
	std::atomic<float> liquid_update{1.0};
	std::atomic<int> block_send_optimize_distance{4};
	std::atomic<bool> server_side_occlusion_culling{true};
	std::atomic<int> csm_flavour_limits{3};
	std::atomic<int> csm_flavour_noderange_limit{8};

/* Security */
	std::atomic<bool> secure_enable_security{true};

/* Advanced */

/** Profiling */
	std::atomic<bool> profiler_load{false};
	std::atomic<ProfilerDefaultReportFormat> profiler_default_report_format{ProfilerDefaultReportFormat::Txt};

/*** Instrumentation */
	std::atomic<bool> instrument_entity{true};
	std::atomic<bool> instrument_abm{true};
	std::atomic<bool> instrument_lbm{true};
	std::atomic<bool> instrument_chatcommand{true};
	std::atomic<bool> instrument_global_callback{true};

/**** Advanced */
	std::atomic<bool> instrument_builtin{false};
	std::atomic<bool> instrument_profiler{false};

/*
 * Client and Server
 */
	std::atomic<Language> language{Language::_};
	std::atomic<DebugLogLevel> debug_log_level{DebugLogLevel::Action};
	std::atomic<bool> enable_ipv6{true};

/* Advanced */
	std::atomic<int> curl_timeout{5000};
	std::atomic<int> curl_parallel_limit{8};
	std::atomic<int> curl_file_download_timeout{300000};
	std::atomic<bool> high_precision_fpu{true};
	std::atomic<int> main_menu_game_mgr{0};
	std::atomic<int> main_menu_mod_mgr{1};
	std::atomic<int> profiler_print_interval{0};

/*
 * Mapgen
 */
	std::atomic<MgName> mg_name{MgName::V7};
	std::atomic<int> water_level{1};
	std::atomic<int> max_block_generate_distance{6};
	std::atomic<int> mapgen_limit{31000};
	std::atomic<bool> projecting_dungeons{true};

/* Biome API temperature and humidity noise parameters */

/* Mapgen V5 */
	std::atomic<float> mgv5_cave_width{0.125};
	std::atomic<int> mgv5_large_cave_depth{-256};
	std::atomic<int> mgv5_lava_depth{-256};
	std::atomic<int> mgv5_cavern_limit{-256};
	std::atomic<int> mgv5_cavern_taper{256};
	std::atomic<float> mgv5_cavern_threshold{0.7};

/** Noises */

/* Mapgen V6 */
	std::atomic<float> mgv6_freq_desert{0.45};
	std::atomic<float> mgv6_freq_beach{0.15};

/** Noises */

/* Mapgen V7 */
	std::atomic<int> mgv7_mount_zero_level{0};
	std::atomic<float> mgv7_cave_width{0.09};
	std::atomic<int> mgv7_large_cave_depth{-33};
	std::atomic<int> mgv7_lava_depth{-256};
	std::atomic<float> mgv7_float_mount_density{0.6};
	std::atomic<float> mgv7_float_mount_height{128.0};
	std::atomic<int> mgv7_floatland_level{1280};
	std::atomic<int> mgv7_shadow_limit{1024};
	std::atomic<int> mgv7_cavern_limit{-256};
	std::atomic<int> mgv7_cavern_taper{256};
	std::atomic<float> mgv7_cavern_threshold{0.7};

/** Noises */

/* Mapgen Carpathian */
	std::atomic<float> mgcarpathian_cave_width{0.09};
	std::atomic<int> mgcarpathian_large_cave_depth{-33};
	std::atomic<int> mgcarpathian_lava_depth{-256};
	std::atomic<int> mgcarpathian_cavern_limit{-256};
	std::atomic<int> mgcarpathian_cavern_taper{256};
	std::atomic<float> mgcarpathian_cavern_threshold{0.7};

/** Noises */

/* Mapgen Flat */
	std::atomic<int> mgflat_ground_level{8};
	std::atomic<int> mgflat_large_cave_depth{-33};
	std::atomic<int> mgflat_lava_depth{-256};
	std::atomic<float> mgflat_cave_width{0.09};
	std::atomic<float> mgflat_lake_threshold{-0.45};
	std::atomic<float> mgflat_lake_steepness{48.0};
	std::atomic<float> mgflat_hill_threshold{0.45};
	std::atomic<float> mgflat_hill_steepness{64.0};

/** Noises */

/* Mapgen Fractal */
	std::atomic<float> mgfractal_cave_width{0.09};
	std::atomic<int> mgfractal_large_cave_depth{-33};
	std::atomic<int> mgfractal_lava_depth{-256};
	std::atomic<int> mgfractal_fractal{1};
	std::atomic<int> mgfractal_iterations{11};
	std::atomic<float> mgfractal_slice_w{0.0};
	std::atomic<float> mgfractal_julia_x{0.33};
	std::atomic<float> mgfractal_julia_y{0.33};
	std::atomic<float> mgfractal_julia_z{0.33};
	std::atomic<float> mgfractal_julia_w{0.33};

/** Noises */

/* Mapgen Valleys */
	std::atomic<int> mgvalleys_altitude_chill{90};
	std::atomic<int> mgvalleys_large_cave_depth{-33};
	std::atomic<int> mgvalleys_lava_features{0};
	std::atomic<int> mgvalleys_massive_cave_depth{-256};
	std::atomic<int> mgvalleys_river_depth{4};
	std::atomic<int> mgvalleys_river_size{5};
	std::atomic<int> mgvalleys_water_features{0};
	std::atomic<float> mgvalleys_cave_width{0.09};

/** Noises */

/* Advanced */
	std::atomic<int> chunksize{5};
	std::atomic<bool> enable_mapgen_debug_info{false};
	std::atomic<int> emergequeue_limit_total{256};
	std::atomic<int> emergequeue_limit_diskonly{32};
	std::atomic<int> emergequeue_limit_generate{32};
	std::atomic<int> num_emerge_threads{1};

};

extern const BuiltinSettings &builtin_settings;
extern StaticSettingsManager builtin_settings_manager;
