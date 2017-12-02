if !exists("main_syntax")
  if version < 600
    syntax clear
  elseif exists("b:current_syntax")
    finish
  endif
  let main_syntax = 'apdu'
endif

syn match apduCmd "\v^\s*([0-9a-fA-F]{2}\s*){4,5}" contains=apduFour
syn match apduFour display contained "\v^\s*([0-9a-fA-F]{2}\s*){4}" contains=apduTwo
syn match apduTwo display contained "\v^\s*([0-9a-fA-F]{2}\s*){2}" contains=apduOne
syn match apduOne display contained "\v^\s*([0-9a-fA-F]{2}\s*){1}"

syn match apduComment "\v^\s*(//|#).*$"

hi link apduCmd Constant
hi link apduFour Structure
hi link apduTwo String
hi link apduOne Label

hi link apduComment Comment
