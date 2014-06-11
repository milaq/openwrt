#
# Copyright (C) 2012 Michel Stempin <michel.stempin@wanadoo.fr>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/MPR_A1
	NAME:=HAME MPR-A1
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-ledtrig-netdev kmod-ledtrig-timer
endef

define Profile/MPR_A1/Description
	Package set for HAME MPR-A1 board
endef

$(eval $(call Profile,MPR_A1))
