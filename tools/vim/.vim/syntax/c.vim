"所有字母大写的符号视为宏
syn match cMacro display "\<\(wx\)\?[A-Z_][A-Z0-9_]*\>"
hi def link cMacro Macro

"highlight functions
syn match cFunction display "\(#\s*define\s\+\)\@<!\(#\s*pragma\s\+\)\@<!\(\<defined\>\)\@!\<[a-zA-Z_]\w*\>\s*\(\n\s*\)*("ms=s,me=e-1
hi def link cFunction Function

"highlight keywords in jcvm
syn keyword jcvmkeys    pobj_t pcobj_t pvoid_t lpvoid_t pbyte_t lpbyte_t pword_t lpword_t pdword_t lpdword_t pint16_t pint32_t lpint16_t lpint32_t pcvoid_t lpcvoid_t pcbyte_t lpcbyte_t pcword_t lpcword_t pcdword_t lpcdword_t pcint16_t pcint32_t lpcint16_t lpcint32_t gpvoid_t gpbyte_t gpword_t gpdword_t gpcvoid_t gpcbyte_t gpcword_t gpcdword_t _nsize_t  _noffset_t  _rsize_t  _roffset_t byte_t word_t dword_t bool_t jbool jbyte jshort jint objref_t

hi def link jcvmkeys Type

so <sfile>:p:h/operators_hi.vim
