TEMPLATE = subdirs
contains(style-plugins, windows)	:SUBDIRS += windows
contains(style-plugins, motif)		:SUBDIRS += motif
contains(style-plugins, platinum)	:SUBDIRS += platinum
contains(style-plugins, motifplus)	:SUBDIRS += motifplus
contains(style-plugins, cde)		:SUBDIRS += cde
contains(style-plugins, sgi)		:SUBDIRS += sgi
contains(style-plugins, compact)	:SUBDIRS += compact
contains(style-plugins, aqua)		:SUBDIRS += aqua
contains(style-plugins, mac)		:SUBDIRS += mac
contains(style-plugins, windowsxp)	:SUBDIRS += windowsxp