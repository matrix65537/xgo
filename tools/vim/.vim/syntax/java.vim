"所有字母大写的符号视为宏
syn match javaConstant display "\<[A-Z_][A-Z0-9_]*\>"
hi def link javaConstant Macro


so <sfile>:p:h/function_hi.vim
so <sfile>:p:h/operators_hi.vim
