""
 " @brief 
 " @file latexcomplete.vim
 " @author Hulei
 " @version 1.0
 " @date 2013-12-07
 " @copyright Copyright(C) 2013 Feitian Technologies Co., Ltd. All rights reserved.
 "
""
 " Changes:
 "  ** version 1.0 2013-12-07 Hulei **
 "      1. first version

python << PYTHONEOF

import vim
basedir = vim.eval("$vimfiles") + "/bundle/latex_complete/autoload"
execfile(basedir + "/latexcomplete.py", globals())

PYTHONEOF

function! DebugMsg(msg)
    "let g:debug_str = a:msg
    "exe 'python debugmsg()'
endfunction

function! latexcomplete#Complete(findstart, base)
    if a:findstart
        let line = getline('.')
        let idx = col('.') - 1
        let hasleftbrace = 0
        while idx > 0
            let idx -= 1
            let c = line[idx]
            if c =~ '\v[a-zA-Z]'
                continue
            elseif c == "\\"
                return idx + 1
            elseif c == '{'
                if hasleftbrace
                    break
                else
                    let hasleftbrace = 1
                endif
            else
                break
            endif
        endwhile
        return -1
    else
        let g:latex_base_text = a:base
        execute 'python latex_complete()'
        call sort(g:latex_complete_dict)
        return g:latex_complete_dict
    endif
endfunction


