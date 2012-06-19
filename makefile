#
# Makefile for 'autoPilot'.
#
# Type 'make' or 'make autoPilot' to create the binary.
# Type 'make clean' or 'make clear' to delete all temporaries.
# Type 'make run' to execute the binary.
# Type 'make debug' to debug the binary using gdb(1).
#

# build target specs
CC = g++
CFLAGS = -O3 
OUT_DIR = release
LIBS = -lpthread

# first target entry is the target invoked when typing 'make'
default: autoPilot

autoPilot: $(OUT_DIR)/ch6dm_linux.c.o $(OUT_DIR)/adcAccess.cpp.o $(OUT_DIR)/gumstix_autopilot.cpp.o $(OUT_DIR)/SystemThreads.cpp.o $(OUT_DIR)/Utility.cpp.o
	@echo -n 'Linking autoPilot... '
	@$(CC) $(CFLAGS) -o autoPilot $(OUT_DIR)/ch6dm_linux.c.o $(OUT_DIR)/adcAccess.cpp.o $(OUT_DIR)/gumstix_autopilot.cpp.o $(OUT_DIR)/SystemThreads.cpp.o $(OUT_DIR)/Utility.cpp.o $(LIBS)
	@echo Done.

$(OUT_DIR)/ch6dm_linux.c.o: ch6dm_linux.c ch6dm_linux.h SharedMemory.h \
 SystemConfig.h
	@echo -n 'Compiling ch6dm_linux.c... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/ch6dm_linux.c.o -c ch6dm_linux.c
	@echo Done.

$(OUT_DIR)/adcAccess.cpp.o: adcAccess.cpp adcAccess.h SharedMemory.h \
 SystemConfig.h
	@echo -n 'Compiling adcAccess.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/adcAccess.cpp.o -c adcAccess.cpp
	@echo Done.

$(OUT_DIR)/gumstix_autopilot.cpp.o: gumstix_autopilot.cpp SystemStatus.h \
 SystemConfig.h SharedMemory.h SystemThreads.h adcAccess.h Utility.h \
 mavlink/common/mavlink.h mavlink/common/version.h \
 mavlink/common/common.h mavlink/common/../protocol.h \
 mavlink/common/../mavlink_types.h mavlink/common/../mavlink_helpers.h \
 mavlink/common/../checksum.h mavlink/common/./mavlink_msg_heartbeat.h \
 mavlink/common/./mavlink_msg_sys_status.h \
 mavlink/common/./mavlink_msg_system_time.h \
 mavlink/common/./mavlink_msg_ping.h \
 mavlink/common/./mavlink_msg_change_operator_control.h \
 mavlink/common/./mavlink_msg_change_operator_control_ack.h \
 mavlink/common/./mavlink_msg_auth_key.h \
 mavlink/common/./mavlink_msg_set_mode.h \
 mavlink/common/./mavlink_msg_param_request_read.h \
 mavlink/common/./mavlink_msg_param_request_list.h \
 mavlink/common/./mavlink_msg_param_value.h \
 mavlink/common/./mavlink_msg_param_set.h \
 mavlink/common/./mavlink_msg_gps_raw_int.h \
 mavlink/common/./mavlink_msg_gps_status.h \
 mavlink/common/./mavlink_msg_scaled_imu.h \
 mavlink/common/./mavlink_msg_raw_imu.h \
 mavlink/common/./mavlink_msg_raw_pressure.h \
 mavlink/common/./mavlink_msg_scaled_pressure.h \
 mavlink/common/./mavlink_msg_attitude.h \
 mavlink/common/./mavlink_msg_attitude_quaternion.h \
 mavlink/common/./mavlink_msg_local_position_ned.h \
 mavlink/common/./mavlink_msg_global_position_int.h \
 mavlink/common/./mavlink_msg_rc_channels_scaled.h \
 mavlink/common/./mavlink_msg_rc_channels_raw.h \
 mavlink/common/./mavlink_msg_servo_output_raw.h \
 mavlink/common/./mavlink_msg_mission_request_partial_list.h \
 mavlink/common/./mavlink_msg_mission_write_partial_list.h \
 mavlink/common/./mavlink_msg_mission_item.h \
 mavlink/common/./mavlink_msg_mission_request.h \
 mavlink/common/./mavlink_msg_mission_set_current.h \
 mavlink/common/./mavlink_msg_mission_current.h \
 mavlink/common/./mavlink_msg_mission_request_list.h \
 mavlink/common/./mavlink_msg_mission_count.h \
 mavlink/common/./mavlink_msg_mission_clear_all.h \
 mavlink/common/./mavlink_msg_mission_item_reached.h \
 mavlink/common/./mavlink_msg_mission_ack.h \
 mavlink/common/./mavlink_msg_set_gps_global_origin.h \
 mavlink/common/./mavlink_msg_gps_global_origin.h \
 mavlink/common/./mavlink_msg_set_local_position_setpoint.h \
 mavlink/common/./mavlink_msg_local_position_setpoint.h \
 mavlink/common/./mavlink_msg_global_position_setpoint_int.h \
 mavlink/common/./mavlink_msg_set_global_position_setpoint_int.h \
 mavlink/common/./mavlink_msg_safety_set_allowed_area.h \
 mavlink/common/./mavlink_msg_safety_allowed_area.h \
 mavlink/common/./mavlink_msg_set_roll_pitch_yaw_thrust.h \
 mavlink/common/./mavlink_msg_set_roll_pitch_yaw_speed_thrust.h \
 mavlink/common/./mavlink_msg_roll_pitch_yaw_thrust_setpoint.h \
 mavlink/common/./mavlink_msg_roll_pitch_yaw_speed_thrust_setpoint.h \
 mavlink/common/./mavlink_msg_set_quad_motors_setpoint.h \
 mavlink/common/./mavlink_msg_set_quad_swarm_roll_pitch_yaw_thrust.h \
 mavlink/common/./mavlink_msg_nav_controller_output.h \
 mavlink/common/./mavlink_msg_state_correction.h \
 mavlink/common/./mavlink_msg_request_data_stream.h \
 mavlink/common/./mavlink_msg_data_stream.h \
 mavlink/common/./mavlink_msg_manual_control.h \
 mavlink/common/./mavlink_msg_rc_channels_override.h \
 mavlink/common/./mavlink_msg_vfr_hud.h \
 mavlink/common/./mavlink_msg_command_long.h \
 mavlink/common/./mavlink_msg_command_ack.h \
 mavlink/common/./mavlink_msg_local_position_ned_system_global_offset.h \
 mavlink/common/./mavlink_msg_hil_state.h \
 mavlink/common/./mavlink_msg_hil_controls.h \
 mavlink/common/./mavlink_msg_hil_rc_inputs_raw.h \
 mavlink/common/./mavlink_msg_optical_flow.h \
 mavlink/common/./mavlink_msg_global_vision_position_estimate.h \
 mavlink/common/./mavlink_msg_vision_position_estimate.h \
 mavlink/common/./mavlink_msg_vision_speed_estimate.h \
 mavlink/common/./mavlink_msg_vicon_position_estimate.h \
 mavlink/common/./mavlink_msg_memory_vect.h \
 mavlink/common/./mavlink_msg_debug_vect.h \
 mavlink/common/./mavlink_msg_named_value_float.h \
 mavlink/common/./mavlink_msg_named_value_int.h \
 mavlink/common/./mavlink_msg_statustext.h \
 mavlink/common/./mavlink_msg_debug.h
	@echo -n 'Compiling gumstix_autopilot.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/gumstix_autopilot.cpp.o -c gumstix_autopilot.cpp
	@echo Done.

$(OUT_DIR)/SystemThreads.cpp.o: SystemThreads.cpp SystemThreads.h \
 adcAccess.h SharedMemory.h SystemConfig.h Utility.h \
 mavlink/common/mavlink.h mavlink/common/version.h \
 mavlink/common/common.h mavlink/common/../protocol.h \
 mavlink/common/../mavlink_types.h mavlink/common/../mavlink_helpers.h \
 mavlink/common/../checksum.h mavlink/common/./mavlink_msg_heartbeat.h \
 mavlink/common/./mavlink_msg_sys_status.h \
 mavlink/common/./mavlink_msg_system_time.h \
 mavlink/common/./mavlink_msg_ping.h \
 mavlink/common/./mavlink_msg_change_operator_control.h \
 mavlink/common/./mavlink_msg_change_operator_control_ack.h \
 mavlink/common/./mavlink_msg_auth_key.h \
 mavlink/common/./mavlink_msg_set_mode.h \
 mavlink/common/./mavlink_msg_param_request_read.h \
 mavlink/common/./mavlink_msg_param_request_list.h \
 mavlink/common/./mavlink_msg_param_value.h \
 mavlink/common/./mavlink_msg_param_set.h \
 mavlink/common/./mavlink_msg_gps_raw_int.h \
 mavlink/common/./mavlink_msg_gps_status.h \
 mavlink/common/./mavlink_msg_scaled_imu.h \
 mavlink/common/./mavlink_msg_raw_imu.h \
 mavlink/common/./mavlink_msg_raw_pressure.h \
 mavlink/common/./mavlink_msg_scaled_pressure.h \
 mavlink/common/./mavlink_msg_attitude.h \
 mavlink/common/./mavlink_msg_attitude_quaternion.h \
 mavlink/common/./mavlink_msg_local_position_ned.h \
 mavlink/common/./mavlink_msg_global_position_int.h \
 mavlink/common/./mavlink_msg_rc_channels_scaled.h \
 mavlink/common/./mavlink_msg_rc_channels_raw.h \
 mavlink/common/./mavlink_msg_servo_output_raw.h \
 mavlink/common/./mavlink_msg_mission_request_partial_list.h \
 mavlink/common/./mavlink_msg_mission_write_partial_list.h \
 mavlink/common/./mavlink_msg_mission_item.h \
 mavlink/common/./mavlink_msg_mission_request.h \
 mavlink/common/./mavlink_msg_mission_set_current.h \
 mavlink/common/./mavlink_msg_mission_current.h \
 mavlink/common/./mavlink_msg_mission_request_list.h \
 mavlink/common/./mavlink_msg_mission_count.h \
 mavlink/common/./mavlink_msg_mission_clear_all.h \
 mavlink/common/./mavlink_msg_mission_item_reached.h \
 mavlink/common/./mavlink_msg_mission_ack.h \
 mavlink/common/./mavlink_msg_set_gps_global_origin.h \
 mavlink/common/./mavlink_msg_gps_global_origin.h \
 mavlink/common/./mavlink_msg_set_local_position_setpoint.h \
 mavlink/common/./mavlink_msg_local_position_setpoint.h \
 mavlink/common/./mavlink_msg_global_position_setpoint_int.h \
 mavlink/common/./mavlink_msg_set_global_position_setpoint_int.h \
 mavlink/common/./mavlink_msg_safety_set_allowed_area.h \
 mavlink/common/./mavlink_msg_safety_allowed_area.h \
 mavlink/common/./mavlink_msg_set_roll_pitch_yaw_thrust.h \
 mavlink/common/./mavlink_msg_set_roll_pitch_yaw_speed_thrust.h \
 mavlink/common/./mavlink_msg_roll_pitch_yaw_thrust_setpoint.h \
 mavlink/common/./mavlink_msg_roll_pitch_yaw_speed_thrust_setpoint.h \
 mavlink/common/./mavlink_msg_set_quad_motors_setpoint.h \
 mavlink/common/./mavlink_msg_set_quad_swarm_roll_pitch_yaw_thrust.h \
 mavlink/common/./mavlink_msg_nav_controller_output.h \
 mavlink/common/./mavlink_msg_state_correction.h \
 mavlink/common/./mavlink_msg_request_data_stream.h \
 mavlink/common/./mavlink_msg_data_stream.h \
 mavlink/common/./mavlink_msg_manual_control.h \
 mavlink/common/./mavlink_msg_rc_channels_override.h \
 mavlink/common/./mavlink_msg_vfr_hud.h \
 mavlink/common/./mavlink_msg_command_long.h \
 mavlink/common/./mavlink_msg_command_ack.h \
 mavlink/common/./mavlink_msg_local_position_ned_system_global_offset.h \
 mavlink/common/./mavlink_msg_hil_state.h \
 mavlink/common/./mavlink_msg_hil_controls.h \
 mavlink/common/./mavlink_msg_hil_rc_inputs_raw.h \
 mavlink/common/./mavlink_msg_optical_flow.h \
 mavlink/common/./mavlink_msg_global_vision_position_estimate.h \
 mavlink/common/./mavlink_msg_vision_position_estimate.h \
 mavlink/common/./mavlink_msg_vision_speed_estimate.h \
 mavlink/common/./mavlink_msg_vicon_position_estimate.h \
 mavlink/common/./mavlink_msg_memory_vect.h \
 mavlink/common/./mavlink_msg_debug_vect.h \
 mavlink/common/./mavlink_msg_named_value_float.h \
 mavlink/common/./mavlink_msg_named_value_int.h \
 mavlink/common/./mavlink_msg_statustext.h \
 mavlink/common/./mavlink_msg_debug.h ch6dm_linux.h tiC2000.h
	@echo -n 'Compiling SystemThreads.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/SystemThreads.cpp.o -c SystemThreads.cpp
	@echo Done.

$(OUT_DIR)/Utility.cpp.o: Utility.cpp Utility.h
	@echo -n 'Compiling Utility.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/Utility.cpp.o -c Utility.cpp
	@echo Done.

run:
	./autoPilot 

debug:
	gdb ./autoPilot

clean:
	@echo -n 'Removing all temporary binaries... '
	@rm -f autoPilot $(OUT_DIR)/*.o
	@echo Done.

clear:
	@echo -n 'Removing all temporary binaries... '
	@rm -f autoPilot $(OUT_DIR)/*.o
	@echo Done.

