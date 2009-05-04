# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# Header: $

EAPI=2
inherit games bzr

DESCRIPTION="3D arcade-puzzle based on Mr.Driller"
HOMEPAGE="http://xdriller.sourceforge.net/"
SRC_URI="mirror://sourceforge/${PN}/xdriller-0.1.tar.bz2"
EBZR_REPO_URI="bzr://xdriller.bzr.sourceforge.net/bzrroot/xdriller"
EBZR_BRANCH=""

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

RDEPEND="dev-games/ogre
	media-libs/sdl-mixer
	dev-games/ois"
DEPEND="${RDEPEND}
    dev-util/pkgconfig
	dev-util/bzr"

src_unpack() {
	bzr_src_unpack
	unpack xdriller-0.1.tar.bz2
}

src_compile() {
	emake || die "emake failed"
}

src_install() {
	dogamesbin ${PN} || die "dogamesbin failed"

	insinto "${GAMES_DATADIR}"/${PN}
	doins -r xdriller-0.1/media/* || die "doins failed"
	doins -r xdriller-0.1/default_config || die "doins failed"

	dodoc README

	doicon  ${PN}.png
	make_desktop_entry ${PN} "Xdriller" ${PN}
}

