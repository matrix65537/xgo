syn match hexComment ";.*"
hi def link hexComment      Comment
hi def link hexAdress       Function

syn match hexEvenData "\(^:[0-9a-fA-F]\{10}\([0-9a-fA-F]\{4}\)*\)\@<=\([0-9a-fA-F][0-9a-fA-F]\)\([0-9a-fA-F]*[0-9a-fA-F]\{2}$\)\@="

hi def link hexEvenData     Label
