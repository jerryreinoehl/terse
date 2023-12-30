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
sha256sums=("a703898fc27f8b88ba7a5377836e2fb55b981262c9fcb986240b22bb63b9cc49")

build() {
	cd "$pkgname-$pkgver"
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
