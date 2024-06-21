IMG_NAME=bandnoticeboard/nottoboard:forestmq-0.1.2

# Development requirements
# This will install all the required development libraries on a Mac.
install_ulfius_mac:
	brew install gnutls
	brew install ulfius
	brew install check
	brew install cmocka
	brew install doxygen

docs_init:
	doxygen -g Doxyfile

docker-build:
	docker build --no-cache --tag $(IMG_NAME) .

docker-run:
	docker run --env-file ../.env --rm -p 8001:8001 $(IMG_NAME)

docker-push:
	docker push $(IMG_NAME)

docker-remove:
	docker rmi $(IMG_NAME)