all:
	g++ -std=c++11 -O2   main.cpp lexing.cpp io.cpp token.cpp parsing.cpp exceptionHandler.cpp IRVisitor.cpp -I ./includes

fc:
	fc A\myoutput1.txt A\output1.txt
	fc A\myoutput2.txt A\output2.txt
	fc A\myoutput3.txt A\output3.txt
	fc A\myoutput4.txt A\output4.txt
	fc A\myoutput5.txt A\output5.txt
	fc A\myoutput6.txt A\output6.txt
	fc A\myoutput7.txt A\output7.txt
	fc A\myoutput8.txt A\output8.txt
	fc A\myoutput9.txt A\output9.txt
	fc A\myoutput10.txt A\output10.txt
	fc A\myoutput11.txt A\output11.txt
	fc A\myoutput12.txt A\output12.txt
	fc A\myoutput13.txt A\output13.txt
	fc A\myoutput14.txt A\output14.txt
	fc A\myoutput15.txt A\output15.txt
	fc A\myoutput16.txt A\output16.txt
	fc A\myoutput17.txt A\output17.txt
	fc A\myoutput18.txt A\output18.txt
	fc A\myoutput19.txt A\output19.txt
	fc A\myoutput20.txt A\output20.txt
	fc A\myoutput21.txt A\output21.txt
	fc A\myoutput22.txt A\output22.txt
	fc A\myoutput23.txt A\output23.txt
	fc A\myoutput24.txt A\output24.txt
	fc A\myoutput25.txt A\output25.txt
	fc A\myoutput26.txt A\output26.txt
	fc A\myoutput27.txt A\output27.txt
	fc A\myoutput28.txt A\output28.txt
	fc A\myoutput29.txt A\output29.txt
	fc A\myoutput30.txt A\output30.txt
	fc B\myoutput1.txt B\output1.txt
	fc B\myoutput2.txt B\output2.txt
	fc B\myoutput3.txt B\output3.txt
	fc B\myoutput4.txt B\output4.txt
	fc B\myoutput5.txt B\output5.txt
	fc B\myoutput6.txt B\output6.txt
	fc B\myoutput7.txt B\output7.txt
	fc B\myoutput8.txt B\output8.txt
	fc B\myoutput9.txt B\output9.txt
	fc B\myoutput10.txt B\output10.txt
	fc B\myoutput11.txt B\output11.txt
	fc B\myoutput12.txt B\output12.txt
	fc B\myoutput13.txt B\output13.txt
	fc B\myoutput14.txt B\output14.txt
	fc B\myoutput15.txt B\output15.txt
	fc B\myoutput16.txt B\output16.txt
	fc B\myoutput17.txt B\output17.txt
	fc B\myoutput18.txt B\output18.txt
	fc B\myoutput19.txt B\output19.txt
	fc B\myoutput20.txt B\output20.txt
	fc B\myoutput21.txt B\output21.txt
	fc B\myoutput22.txt B\output22.txt
	fc B\myoutput23.txt B\output23.txt
	fc B\myoutput24.txt B\output24.txt
	fc B\myoutput25.txt B\output25.txt
	fc B\myoutput26.txt B\output26.txt
	fc B\myoutput27.txt B\output27.txt
	fc B\myoutput28.txt B\output28.txt
	fc B\myoutput29.txt B\output29.txt
	fc B\myoutput30.txt B\output30.txt
	fc C\myoutput1.txt C\output1.txt
	fc C\myoutput2.txt C\output2.txt
	fc C\myoutput3.txt C\output3.txt
	fc C\myoutput4.txt C\output4.txt
	fc C\myoutput5.txt C\output5.txt
	fc C\myoutput6.txt C\output6.txt
	fc C\myoutput7.txt C\output7.txt
	fc C\myoutput8.txt C\output8.txt
	fc C\myoutput9.txt C\output9.txt
	fc C\myoutput10.txt C\output10.txt
	fc C\myoutput11.txt C\output11.txt
	fc C\myoutput12.txt C\output12.txt
	fc C\myoutput13.txt C\output13.txt
	fc C\myoutput14.txt C\output14.txt
	fc C\myoutput15.txt C\output15.txt
	fc C\myoutput16.txt C\output16.txt
	fc C\myoutput17.txt C\output17.txt
	fc C\myoutput18.txt C\output18.txt
	fc C\myoutput19.txt C\output19.txt
	fc C\myoutput20.txt C\output20.txt
	fc C\myoutput21.txt C\output21.txt
	fc C\myoutput22.txt C\output22.txt
	fc C\myoutput23.txt C\output23.txt
	fc C\myoutput24.txt C\output24.txt
	fc C\myoutput25.txt C\output25.txt
	fc C\myoutput26.txt C\output26.txt
	fc C\myoutput27.txt C\output27.txt
	fc C\myoutput28.txt C\output28.txt
	fc C\myoutput29.txt C\output29.txt
	fc C\myoutput30.txt C\output30.txt

ll:
	llvm-link myoutput1.ll lib.ll -S -o out.ll
	llvm-link myoutput2.ll lib.ll -S -o out.ll
	llvm-link myoutput3.ll lib.ll -S -o out.ll
	llvm-link myoutput4.ll lib.ll -S -o out.ll
	llvm-link myoutput5.ll lib.ll -S -o out.ll
	llvm-link myoutput6.ll lib.ll -S -o out.ll
	llvm-link myoutput7.ll lib.ll -S -o out.ll
	llvm-link myoutput8.ll lib.ll -S -o out.ll
	llvm-link myoutput9.ll lib.ll -S -o out.ll
	llvm-link myoutput10.ll lib.ll -S -o out.ll
	llvm-link myoutput11.ll lib.ll -S -o out.ll
	llvm-link myoutput12.ll lib.ll -S -o out.ll
	llvm-link myoutput13.ll lib.ll -S -o out.ll
	llvm-link myoutput14.ll lib.ll -S -o out.ll
	llvm-link myoutput15.ll lib.ll -S -o out.ll
	llvm-link myoutput16.ll lib.ll -S -o out.ll
	llvm-link myoutput17.ll lib.ll -S -o out.ll
	llvm-link myoutput18.ll lib.ll -S -o out.ll
	llvm-link myoutput19.ll lib.ll -S -o out.ll
	llvm-link myoutput20.ll lib.ll -S -o out.ll
	llvm-link myoutput21.ll lib.ll -S -o out.ll
	llvm-link myoutput22.ll lib.ll -S -o out.ll
	llvm-link myoutput23.ll lib.ll -S -o out.ll
	llvm-link myoutput24.ll lib.ll -S -o out.ll
	llvm-link myoutput25.ll lib.ll -S -o out.ll
	llvm-link myoutput26.ll lib.ll -S -o out.ll
	llvm-link myoutput27.ll lib.ll -S -o out.ll
	llvm-link myoutput28.ll lib.ll -S -o out.ll
	llvm-link myoutput29.ll lib.ll -S -o out.ll
	llvm-link myoutput30.ll lib.ll -S -o out.ll

