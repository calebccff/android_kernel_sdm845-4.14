/* Copyright (c) 2016-2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _CAM_ISP_HW_MGR_INTF_H_
#define _CAM_ISP_HW_MGR_INTF_H_

#include <linux/of.h>
#include <linux/time.h>
#include <linux/list.h>
#include <uapi/media/cam_isp.h>
#include "cam_hw_mgr_intf.h"

/*
 * bit position in resource bitmap
 */
#define CAM_IFE_REG_UPD_CMD_PIX_BIT     0
#define CAM_IFE_REG_UPD_CMD_RDI0_BIT    1
#define CAM_IFE_REG_UPD_CMD_RDI1_BIT    2
#define CAM_IFE_REG_UPD_CMD_RDI2_BIT    3
#define CAM_IFE_REG_UPD_CMD_RDI3_BIT    4
#define CAM_IFE_REG_UPD_CMD_DUAL_PD_BIT 5

/* MAX IFE instance */
#define CAM_IFE_HW_NUM_MAX   4
#define CAM_IFE_RDI_NUM_MAX  4

/**
 *  enum cam_isp_hw_event_type - Collection of the ISP hardware events
 */
enum cam_isp_hw_event_type {
	CAM_ISP_HW_EVENT_ERROR,
	CAM_ISP_HW_EVENT_SOF,
	CAM_ISP_HW_EVENT_REG_UPDATE,
	CAM_ISP_HW_EVENT_EPOCH,
	CAM_ISP_HW_EVENT_EOF,
	CAM_ISP_HW_EVENT_DONE,
	CAM_ISP_HW_EVENT_MAX
};


/**
 * enum cam_isp_hw_err_type - Collection of the ISP error types for
 *                         ISP hardware event CAM_ISP_HW_EVENT_ERROR
 */
enum cam_isp_hw_err_type {
	CAM_ISP_HW_ERROR_NONE,
	CAM_ISP_HW_ERROR_OVERFLOW,
	CAM_ISP_HW_ERROR_P2I_ERROR,
	CAM_ISP_HW_ERROR_VIOLATION,
	CAM_ISP_HW_ERROR_BUSIF_OVERFLOW,
	CAM_ISP_HW_ERROR_CSID_FATAL,
	CAM_ISP_HW_ERROR_CSID_NON_FATAL,
	CAM_ISP_HW_ERROR_MAX,
};

/**
 *  enum cam_isp_hw_stop_cmd - Specify the stop command type
 */
enum cam_isp_hw_stop_cmd {
	CAM_ISP_HW_STOP_AT_FRAME_BOUNDARY,
	CAM_ISP_HW_STOP_IMMEDIATELY,
	CAM_ISP_HW_STOP_MAX,
};

/**
 * struct cam_isp_stop_hw_method - hardware stop method
 *
 * @hw_stop_cmd:               Hardware stop command type information
 *
 */
struct cam_isp_stop_hw_method {
	enum cam_isp_hw_stop_cmd      hw_stop_cmd;
};

struct cam_isp_bw_config_internal_ab {
	uint32_t    usage_type;
	uint32_t    num_rdi;
	uint64_t    left_pix_vote_ab;
	uint64_t    right_pix_vote_ab;
	uint64_t    rdi_vote_ab[CAM_IFE_RDI_NUM_MAX];
};

/**
 * struct cam_isp_bw_config_internal - Internal Bandwidth configuration
 *
 * @usage_type:                 Usage type (Single/Dual)
 * @num_rdi:                    Number of RDI votes
 * @left_pix_vote:              Bandwidth vote for left ISP
 * @right_pix_vote:             Bandwidth vote for right ISP
 * @rdi_vote:                   RDI bandwidth requirements
 */

struct cam_isp_bw_config_internal {
	uint32_t                  usage_type;
	uint32_t                  num_rdi;
	struct cam_isp_bw_vote    left_pix_vote;
	struct cam_isp_bw_vote    right_pix_vote;
	struct cam_isp_bw_vote    rdi_vote[CAM_IFE_RDI_NUM_MAX];
};

/**
 * struct cam_isp_prepare_hw_update_data - hw prepare data
 *
 * @packet_opcode_type:     Packet header opcode in the packet header
 *                          this opcode defines, packet is init packet or
 *                          update packet
 * @bw_config:              BW config information
 * @bw_config_valid:        Flag indicating whether the bw_config at the index
 *                          is valid or not
 * @fps:                    fps vaue which has been updated in hw
 *
 */
struct cam_isp_prepare_hw_update_data {
	uint32_t                              packet_opcode_type;
	struct cam_isp_bw_config_internal     bw_config[CAM_IFE_HW_NUM_MAX];
	struct cam_isp_bw_config_internal_ab  bw_config_ab[CAM_IFE_HW_NUM_MAX];
	bool                                bw_config_valid[CAM_IFE_HW_NUM_MAX];
	uint32_t                            fps;
};


/**
 * struct cam_isp_hw_sof_event_data - Event payload for CAM_HW_EVENT_SOF
 *
 * @timestamp:          Time stamp for the sof event
 * @boot_time:          Boot time stamp for the sof event
 * @irq_mono_boot_time: Time stamp till the execution of IRQ wrt event started
 *
 */
struct cam_isp_hw_sof_event_data {
	uint64_t       timestamp;
	uint64_t       boot_time;
	uint64_t       irq_mono_boot_time;
};

/**
 * struct cam_isp_hw_reg_update_event_data - Event payload for
 *                         CAM_HW_EVENT_REG_UPDATE
 *
 * @timestamp:          Time stamp for the reg update event
 * @irq_mono_boot_time: Time stamp till the execution of IRQ wrt event started
 *
 */
struct cam_isp_hw_reg_update_event_data {
	uint64_t       timestamp;
	uint64_t       irq_mono_boot_time;
};

/**
 * struct cam_isp_hw_epoch_event_data - Event payload for CAM_HW_EVENT_EPOCH
 *
 * @timestamp:          Time stamp for the epoch event
 * @irq_mono_boot_time: Time stamp till the execution of this event started
 *
 */
struct cam_isp_hw_epoch_event_data {
	uint64_t       timestamp;
	uint64_t       irq_mono_boot_time;
};

/**
 * struct cam_isp_hw_done_event_data - Event payload for CAM_HW_EVENT_DONE
 *
 * @num_handles:           Number of resource handeles
 * @resource_handle:       Resource handle array
 * @timestamp:             Timestamp for the buf done event
 * @irq_mono_boot_time:    Time stamp till the execution of this event started
 *
 */
struct cam_isp_hw_done_event_data {
	uint32_t             num_handles;
	uint32_t             resource_handle[
				CAM_NUM_OUT_PER_COMP_IRQ_MAX];
	uint64_t       timestamp;
	uint64_t       irq_mono_boot_time;
};

/**
 * struct cam_isp_hw_eof_event_data - Event payload for CAM_HW_EVENT_EOF
 *
 * @timestamp:             Timestamp for the eof event
 * @irq_mono_boot_time:    Time stamp till the execution of this event started
 *
 */
struct cam_isp_hw_eof_event_data {
	uint64_t       timestamp;
	uint64_t       irq_mono_boot_time;
};

/**
 * struct cam_isp_hw_error_event_data - Event payload for CAM_HW_EVENT_ERROR
 *
 * @error_type:            Error type for the error event
 * @timestamp:             Timestamp for the error event
 * @recovery_enabled:      Identifies if the context needs to recover & reapply
 *                         this request
 * @enable_reg_dump:       enable register dump
 */
struct cam_isp_hw_error_event_data {
	uint32_t             error_type;
	uint64_t             timestamp;
	bool                 recovery_enabled;
	bool                 enable_reg_dump;
};

/* enum cam_isp_hw_mgr_command - Hardware manager command type */
enum cam_isp_hw_mgr_command {
	CAM_ISP_HW_MGR_CMD_IS_RDI_ONLY_CONTEXT,
	CAM_ISP_HW_MGR_CMD_PAUSE_HW,
	CAM_ISP_HW_MGR_CMD_RESUME_HW,
	CAM_ISP_HW_MGR_CMD_SOF_DEBUG,
	CAM_ISP_HW_MGR_CMD_CTX_TYPE,
	CAM_ISP_HW_MGR_CMD_MAX,
};

enum cam_isp_ctx_type {
	CAM_ISP_CTX_FS2 = 1,
	CAM_ISP_CTX_RDI,
	CAM_ISP_CTX_PIX,
	CAM_ISP_CTX_MAX,
};
/**
 * struct cam_isp_hw_cmd_args - Payload for hw manager command
 *
 * @cmd_type               HW command type
 * @sof_irq_enable         To debug if SOF irq is enabled
 * @ctx_type               RDI_ONLY, PIX and RDI, or FS2
 */
struct cam_isp_hw_cmd_args {
	uint32_t                          cmd_type;
	union {
		uint32_t                      sof_irq_enable;
		uint32_t                      ctx_type;
	} u;
};


/**
 * cam_isp_hw_mgr_init()
 *
 * @brief:              Initialization function for the ISP hardware manager
 *
 * @of_node:            Device node input
 * @hw_mgr:             Input/output structure for the ISP hardware manager
 *                          initialization
 *
 */
int cam_isp_hw_mgr_init(struct device_node *of_node,
	struct cam_hw_mgr_intf *hw_mgr);

#endif /* __CAM_ISP_HW_MGR_INTF_H__ */
