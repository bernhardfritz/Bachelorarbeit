# LaTeX Makefile
FILE=Erzeugung_von_Gebirgsketten_Initialpräsentation
all: $(FILE).pdf
.PHONY: clean
clean:
	rm *.aux *.blg *.out *.bbl *.log
$(FILE).pdf: $(FILE).tex
	pdflatex $(FILE)
	pdflatex $(FILE)
