
code_pages_result.log: code_pages.exe
	"code_pages.exe" > "code_pages_result.log"

code_pages.exe: code_pages.c
	cl /Fe"code_pages.exe" "code_pages.c"

