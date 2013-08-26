all:main.tex
	rm *.aux *.log *.toc *.pdf *.js *.prc *.asy *.out *.pre -rf
	rm main-*.* -f
	xelatex main.tex
	#asy main*.asy
	#asy  -texcommand xelatex -tex pdflatex main*.asy
	xelatex main.tex
	rm *.aux *.log *.toc *.js *.prc *.asy *.out *.pre -rf
	rm main-*.* -f
	evince main.pdf &
test:main.pdf
	evince main.pdf
push:
	git add *
	git commit -a
	git push https://github.com/gnustc/d2imdoc master
clean:
	rm *.aux *.log *.toc *.pdf *.js *.prc *.asy *.out *.pre -rf
	rm main-*.* -f
