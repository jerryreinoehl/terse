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
sha256sums=("7355b018e5d4a9b1f3bab6db15d059d46f7255ea49a182086fe8e4a7f725ed25")

build() {
	cd "$pkgname-$pkgver"
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
