# LaTeX Makefile
FILE=Procedural-Generation-of-Mountain-Ranges-Based-on-Geology
all: $(FILE).pdf
.PHONY: clean
clean:
	rm *.aux *.blg *.out *.bbl *.log
$(FILE).pdf: $(FILE).tex
	pdflatex $(FILE)
	bibtex $(FILE)
	pdflatex $(FILE)
	pdflatex $(FILE)
