
code_pages_result.log: code_pages.exe
	"code_pages.exe" > "code_pages_result.log"

code_pages.exe: code_pages.c
	cl /O"code_pages.exe" "code_pages.c"

