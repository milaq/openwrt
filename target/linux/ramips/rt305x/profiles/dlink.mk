#
# Copyright (C) 2014 Micha LaQua <micha.laqua@gmail.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/DIR300B5
	NAME:=D-Link DIR-300(B5)/DIR-600(B5)
	PACKAGES:=\
		kmod-ledtrig-netdev kmod-ledtrig-timer
endef

define Profile/DIR300B5/Description
	Package set for DIR-300/DIR-600 revision B5 based boards
endef

$(eval $(call Profile,DIR300B5))
