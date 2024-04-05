build:
	pip3 install .
	
update:
	rsync -avz ../epiworld/include/epiworld include/

README.md: README.qmd
	quarto render README.qmd

.PHONY: build update