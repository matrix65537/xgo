XPTemplate priority=personal+

XPTvar $author Hulei
XPTvar $email  huleiak47@gmail.com

" if () ** {
" else ** {
XPTvar $BRif     \n

" } ** else {
XPTvar $BRel     \n

" for () ** {
" while () ** {
" do ** {
XPTvar $BRloop   \n

" struct name ** {
XPTvar $BRstc    \n

" int fun() ** {
" class name ** {
XPTvar $BRfun    \n


" int fun ** (
" class name ** (
XPTvar $SPfun      ''

" int fun( ** arg ** )
" if ( ** condition ** )
" for ( ** statement ** )
" [ ** a, b ** ]
" { ** 'k' : 'v' ** }
XPTvar $SParg      ''

" if ** (
" while ** (
" for ** (
XPTvar $SPcmd      ' '

" a ** = ** b
" a = a ** + ** 1
" (a, ** b, ** )
XPTvar $SPop       ' '

XPTvar $DATE_FMT     '%Y-%m-%d'
XPTvar $TIME_FMT     '%H:%M:%S'
XPTvar $DATETIME_FMT '%Y-%m-%d %H:%M:%S'


XPT docf    hint=document\ file
XSET author=$author
XSET date=date()
`$CDL^
 `$CDM^ `^
 `$CDM^ @file `file()^
 `$CDM^ @author `author^
 `$CDM^ @version 1.0
 `$CDM^ @date `date^
 `$CDM^ @copyright Copyright(C) `strftime("%Y")^ `Feitian Technologies Co., Ltd^. All rights reserved.
 `$CDR^

..XPT

XPT date
`date()^

XPT time
`strftime("%H:%M:%S")^

XPT now
`strftime("%Y-%m-%d %H:%M:%S")^

XPT date2
`strftime("%Y年%m月%d日")^

XPT time2
`strftime("%H时%M分%S秒")^

XPT now2
`strftime("%Y年%m月%d日 %H时%M分%S秒")^

XPT hl
`胡磊^

