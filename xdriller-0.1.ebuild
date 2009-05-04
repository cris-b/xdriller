# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# Header: $

EAPI=2
inherit games

DESCRIPTION="3D arcade-puzzle based on Mr.Driller"
HOMEPAGE="http://xdriller.sourceforge.net/"
SRC_URI="mirror://sourceforge/${PN}/${P}.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="amd64 x86"
IUSE=""

RDEPEND="dev-games/ogre
	media-libs/sdl-mixer
	dev-games/ois"
DEPEND="${RDEPEND}
    dev-util/pkgconfig"

src_compile() {
	emake || die "emake failed"
}

src_install() {
	dogamesbin ${PN} || die "dogamesbin failed"

	insinto "${GAMES_DATADIR}"/${PN}
	doins -r media || die "doins failed"
	doins -r default_config || die "doins failed"

	dodoc README

	doicon  ${PN}.png
	make_desktop_entry ${PN} "Xdriller" ${PN}
}

