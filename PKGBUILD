# Maintainer: Jerry Reinoehl <jerryreinoehl@gmail.com>
pkgname=terse
pkgver=0.0.1
pkgrel=1
pkgdesc="Alias commands and their arguments"
arch=(x86_64)
url="https://github.com/jerryreinoehl/terse"
license=("MIT")
makedepends=(gcc)
options=(strip)
source=("$pkgname-$pkgver.tar.gz")
sha256sums=("ea5e3ce5699fa5ed4bae92d1db5f2e674f92c01e9c77f131df80671c3d8da279")

build() {
	cd "$pkgname-$pkgver"
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
