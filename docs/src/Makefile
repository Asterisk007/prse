source_files = $(wildcard *.tex)

documentation: pdf_files
	mkdir -p ../build/
	cp *.pdf ../build/
  
pdf_files: $(source_files)
	texliveonfly $<

clean:
	rm -f *.aux *.log *.out *.pdf *.toc
