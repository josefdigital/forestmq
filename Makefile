# Development requirements
# This will install all the required development libraries on a Mac.
install_ulfius_mac:
	brew install gnutls
	brew install ulfius
	brew install cmocka
	brew install doxygen

docs_init:
	doxygen -g Doxyfile
