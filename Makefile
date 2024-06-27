IMG_NAME=bandnoticeboard/nottoboard:forestmq-0.1.3

build:
	mkdir build
	cmake -S . -B build
	cd build
	make


# Development requirements
# This will install all the required development libraries on a Mac.
install_deps_mac:
	brew install gnutls
	brew install ulfius
	brew install check
	brew install cmocka
	brew install doxygen


install_deps_linux:
	sudo apt install -y libulfius-dev uwsc
	sudo apt install -y libmicrohttpd-dev
	sudo apt install -y libjansson-dev
	sudo apt install -y ibcurl4-gnutls-dev
	sudo apt install -y ibgnutls28-dev
	sudo apt install -y ibgcrypt20-dev
	sudo apt install -y ibsystemd-dev


docs_init:
	doxygen -g Doxyfile

docker-build:
	docker build --tag $(IMG_NAME) .

docker-run:
	docker run -it -p 8005:8005 $(IMG_NAME)

docker-push:
	docker push $(IMG_NAME)

docker-remove:
	docker rmi $(IMG_NAME)

docker_img_exec:
	docker run --rm -it --entrypoint /bin/bash bandnoticeboard/nottoboard:forestmq-0.1.2
