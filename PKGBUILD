# Maintainer: Sam Stuewe <halosghost at archlinux dot info>

pkgname=enlighten-git
pkgver=0
pkgrel=1

pkgdesc='A small tool to modify LCD backlight brightness'
url='https://github.com/HalosGhost/enlighten'
arch=('i686' 'x86_64')
license=('GPL3')

makedepends=('git' 'clang')

source=('git+https://github.com/HalosGhost/enlighten.git')
sha256sums=('SKIP')

pkgver () {
    cd enlighten
    printf '0.r%s.%s' "$(git rev-list --count HEAD)" "$(git log -1 --pretty=format:%h)"
}

prepare () {
    cd enlighten
    ./configure
}

build () {
    cd enlighten
    make
}

package () {
    cd enlighten
    make DESTDIR="$pkgdir" PREFIX="/usr" install
}
