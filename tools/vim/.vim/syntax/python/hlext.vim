syn match pythonConstant display "\<[A-Z_][A-Z0-9_]*\>"
hi def link pythonConstant Macro

so <sfile>:p:h/../function_hi.vim
so <sfile>:p:h/../operators_hi.vim

