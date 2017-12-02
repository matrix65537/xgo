"coding:utf-8
if has('g:vimproject_loaded')
    finish
endif
let g:vimproject_loaded = 1


python << PYTHON_EOF
import vim
import os
import sys
import shutil
from subprocess import call, Popen
from copy import copy
path = os.path

#对于不同编译器的不同错误信息格式，第一项为错误，第二项为警告
_COMPILER_EFM = {
    'msvc' : [
        #error
        [
            r"%f(%l) : %trror C%n: %m",
            r"%f(%l) : fatal %trror C%n: %m",
            r"%s: fatal %trror LNK%n: %m",
            r"%s: %trror LNK%n: %m",
        ],
        #warning
        [
            r"%f(%l) : %tarning C%n: %m",
            r"%s: %tarning LNK%n: %m",
        ],
    ],
    'gcc' : [
        [
            r"%f:%l:%c: %trror:%m",
            r"%f:%l:%c: fatal %trror:%m",
            r"%f:%l: %\(undefined%.%#%\)%\@=%m",
            r"%s:%f:%*[^:]: %\(undefined%.%#%\)%\@=%m",
        ],
        [
            r"%f:%l:%c: %tarning:%m",
        ],
    ],
    'mdk' :[
        [
            r'''%trror:%s''',
            r'''%E"%f"\,%\s%#line%\s%#%l:%\s%#Error:%m''',
            r'''%-Z  %p^''', #mdk的信息缩进了2个空格
            r'''%-C%.%#''',
        ],
        [
            r'''%W"%f"\,%\s%#line%\s%#%l:%\s%#Warning:%m''',
        ],
    ],
    'ads' :[
        [
            r'''"%f"\,%\s%#line%\s%#%l:%\s%#%trror:%m''',
            r'''%trror:%s''',
        ],
        [
            r'''"%f"\,%\s%#line%\s%#%l:%\s%#%tarning:%m''',
        ],
    ],
    'avr' :[
        [
            r'''%trror[%s]:%m''',
            r'''%E"%f"\,%l%\s%#%trror%m''', #avr的列信息在文件信息之前，无法得到
            r'''%+C%[^ ]%#''',
            r'''%-Z%\s%#''',
        ],
        [
            r'''%W"%f"\,%l%\s%#%tarning%m''',
        ],
    ],
    'javac' : [
        [
            r"%+E%f:%l: %\(%[wW]arning:%\|警告：%\)%\@!%m",
            r"%-Z%p^",
            r"%+C%.%#",
        ],
        [
            r"%+W%f:%l: %\(%[wW]arning:%\|警告：%\)%\@=%m",
        ],
    ],
    'common' : [
        [
            r"%f:%l:%m",
            r"%f:%l:%c:%m",
        ],
        [ ],
    ],
    'pclint' : [
        [
            r"%f(%l): %trror %n:%m",
        ],
        [
            r"%f(%l): %tarning %n:%m",
        ]
        ,
    ],
    'pylint' : [
        [
            r"%f:%l: %\([%[EF]%.%#%\)%\@=%m",
        ],
        [
            r"%f:%l: %\([%[WR]%.%#%\)%\@=%m",
        ]
    ],
    'scons' : [
        [
            r"%\(scons: building terminated because of errors.%\)%\@=%m",
        ],
        [],
    ],

    'lex' : [
        [
            r"%f:%l: %trror\,%m",
        ],
        [
            r"%f:%l: %tarning\,%m",
        ],
    ],

    'yacc' :[
        [
            r"%f:%l.%c-%\d%\+: %m",
        ],
        [
            r"%f:%l.%c-%\d%\+: %\(warning:%.%#%\)%\@=%m",
        ],
    ],
}


def formpath(p):
    return p.replace('\\', '/')

def str2vimfmt(s):
    ret = []
    for c in s:
        if c in ['\\', ' ', '|', '"', ',']:
            ret.append('\\')
        ret.append(c)
    return ''.join(ret)

def search_files(pathitems, suffixes):
    ret = []
    for i in xrange(len(pathitems)):
        if pathitems[i] == "*":
            basedir = "/".join(pathitems[:i])
            if not path.isdir(basedir):
                return ret
            for d in os.listdir(basedir):
                if path.isdir(path.join(basedir, d)):
                    items = copy(pathitems)
                    items[i] = d
                    ret.extend(search_files(items, suffixes))
            return ret
        if pathitems[i] == "**":
            basedir = "/".join(pathitems[:i])
            if not path.isdir(basedir):
                return ret
            for root, dirs, files in os.walk(basedir):
                items = copy(pathitems)
                items[i] = path.relpath(root, basedir)
                ret.extend(search_files(items, suffixes))
            return ret
    if isinstance(suffixes, basestring):
        p = "/".join(pathitems) + "/" + suffixes
        if path.isfile(p):
            ret.append(p)
    else:
        basedir = "/".join(pathitems)
        if not path.isdir(basedir):
            return ret
        for f in os.listdir(basedir):
            pp = path.join(basedir, f)
            if ("*" in suffixes or path.splitext(pp)[1].lower() in suffixes) and path.isfile(pp):
                ret.append(formpath(path.normpath(pp)))
    return ret

class VimProject(object):
    def __init__(self):
        self.reset_config()
        self.commit_settings()

    def reset_config(self):
        ext = vim.eval('''expand('%:e')''')
        if not ext:
            ext = ''
        self.projectname = vim.eval('expand("%")')
        if not self.projectname:
            self.projectname = "untitled"
        self.basedir = formpath(vim.eval('getcwd()'))
        self.path = [self.basedir + '/**']
        self.suffix = ['.' + ext]
        self.make= 'make'
        self.execute = ''
        self.files = []
        self.compiler = []
        self.type = ''
        self.warning = False
        self.pause = 0
        self.libtags = 0
        self.tags = []
        self.projectfile = ""
        self.vimcmd = ""
        self.tempdir = formpath(os.environ.get("HOME", vim.eval("$vim")) + "/.vimprj")
        if not os.path.isdir(self.tempdir):
            os.mkdir(self.tempdir)

        if ext in ['py', 'pyw']:
            self.make = 'pylint -r n -f parseable %:p'
            self.compiler = ['pylint']

    def from_file(self, fname):
        if not fname or not path.isfile(fname):
            return
        gl = {}
        try:
            execfile(fname, gl)
        except Exception, e:
            print >> sys.stderr, str(e)
            return
        self.reset_config()
        self.projectfile = formpath(fname)
        self.projectname = path.splitext(path.basename(fname))[0]
        self.basedir = formpath(path.dirname(path.abspath(fname)))
        if gl.has_key('NAME'):
            self.projectname = gl['NAME']
        if gl.has_key('PATH'):
            self.path = map(formpath, map(path.abspath, gl['PATH']))
        if gl.has_key('EXECUTE'):
            self.execute = gl['EXECUTE']
        if gl.has_key('TYPE'):
            self.type = gl['TYPE']
        if gl.has_key('SUFFIX'):
            self.suffix = map(lambda s: s.lower(), gl['SUFFIX'])
        else:
            if self.type == 'python':
                self.suffix = ['.py', '.pyw']
            elif self.type == 'c':
                self.suffix = ['.c', '.h']
            elif self.type == 'cpp':
                self.suffix = ['.c', '.h', '.cpp', '.cc', '.cxx', '.hpp', '.hxx', '.hh']
            elif self.type == 'java':
                self.suffix = ['.java']
            elif self.type == 'latex':
                self.suffix = ['.tex']
            elif self.type == 'vim':
                self.suffix = ['.vim']
        if gl.has_key('MAKE'):
            self.make= gl['MAKE']
        else:
            if self.type == 'python':
                self.make= 'pylint -r n -f parseable %:p'
        if gl.has_key('COMPILER'):
            self.compiler = gl['COMPILER']
        else:
            if self.type in['c', 'cpp']:
                self.compiler = ['msvc', 'gcc', 'scons']
            elif self.type in ['latex']:
                self.compiler = ['common']
            elif self.type == 'java':
                self.compiler = ['javac']
            elif self.type in ['python']:
                self.compiler = ['pylint']
        if gl.has_key('PAUSE'):
            self.pause = gl['PAUSE']
        if gl.has_key('LIBTAGS'):
            self.libtags = gl['LIBTAGS']
        if gl.has_key('TAGS'):
            self.tags = map(formpath, map(path.abspath, gl['TAGS']))
        if gl.has_key('VIMCMD'):
            self.vimcmd = gl['VIMCMD']
        self.commit_settings()

    def get_fname_base(self):
        return ''.join((self.tempdir, '/', self.basedir.replace("/", "_").replace(":", "_"), '_', self.projectname))

    def get_file_list(self):
        return self.get_fname_base() + ".list.tmp"

    def get_tags_fname(self):
        return self.get_fname_base() + '.tags.tmp'

    def get_cscope_fname(self):
        return self.get_fname_base() + '.cscope.tmp'

    def get_make_tmpfile(self):
        return self.get_fname_base() + '.make.tmp'

    def get_grep_tmpfile(self):
        return self.get_fname_base() + '.grep.tmp'

    def get_session_fname(self):
        return self.get_fname_base() + '.session.tmp'

    def add_library_tags(self):
        if not self.libtags:
            return
        ret = []
        if self.type in ['c']:
            inc = os.environ.get("C_INCLUDE_PATH", None)
            if inc:
                ret += map(formpath, inc.split(";"))
        elif self.type in ['cpp']:
            inc = os.environ.get("CPLUS_INCLUDE_PATH", None)
            if inc:
                ret += map(formpath, inc.split(";"))
        for dir in ret:
            vim.command('silent set tags+=%s/tags' % str2vimfmt(dir))

    def add_cscope_database(self):
        if self.type in ['c', 'cpp', 'java']:
            vim.command('silent! cs add %s %s' % (str2vimfmt(self.get_cscope_fname()), str2vimfmt(self.basedir)))

    def load_session_file(self):
        session = self.get_session_fname()
        if self.projectfile and path.isfile(session):
            vim.command(r'''silent so %s''' % str2vimfmt(session))

    def write_session_file(self):
        if self.projectfile:
            session_fname = self.get_session_fname()
            vim.command(r'''silent mks! %s''' % str2vimfmt(session_fname))

    def commit_settings(self):
        enc = vim.eval('&encoding')
        iswin = int(vim.eval('g:isWin'))
        vim.command('''silent set path=.,%s''' % (','.join(map(str2vimfmt, self.path))))
        if iswin:
            vim.command(r'''silent set makeprg=''' + str2vimfmt(r'''%s $* 2>&1 \| recoding %s > "%s" && cat "%s"''' % (self.make, enc, self.get_make_tmpfile(), self.get_make_tmpfile())))
        else:
            vim.command(r'''silent set makeprg=''' + str2vimfmt(r'''%s $* 2>&1 \| cat > '%s' && cat '%s' ''' % (self.make, self.get_make_tmpfile(), self.get_make_tmpfile())))
        self.update_compiler_efm()
        if iswin:
            vim.command('silent set grepprg=' + str2vimfmt(r'''cat "%s" \| pyargs pygrep -HnCS "$*" 2>&1 \| recoding %s > "%s" && cat "%s"''' % (self.get_file_list(), enc, self.get_grep_tmpfile(), self.get_grep_tmpfile())))
        else:
            vim.command('silent set grepprg=' + str2vimfmt(r'''cat '%s' \| xargs pygrep -HnCS '$*' 2>&1 \| recoding %s > '%s' && cat '%s' ''' % (self.get_file_list(), enc, self.get_grep_tmpfile(), self.get_grep_tmpfile())))
        vim.command('silent set grepformat=%f:%l:%c:%m,%f:%l:%m')
        vim.command('silent set tags=%s' % ','.join(map(str2vimfmt, [self.get_tags_fname()] + self.tags)))
        self.add_library_tags()
        self.add_cscope_database()
        if self.vimcmd:
            vim.command(self.vimcmd)

    def update_compiler_efm(self):
        fmts = []
        if not self.compiler:
            self.compiler = ['common']
        for compiler in self.compiler:
            if compiler in _COMPILER_EFM:
                if self.warning:
                    fmts += _COMPILER_EFM[compiler][1]
                fmts += _COMPILER_EFM[compiler][0]
            else:
                vim.command("silent compiler %s" % compiler)
                break
        vim.command(r"silent set efm=%s" % ','.join(map(str2vimfmt, fmts)))

    def load_make_result(self):
        if path.isfile(self.get_make_tmpfile()):
            cwd = formpath(vim.eval('getcwd()'))
            vim.command('silent cd ' + str2vimfmt(self.basedir))
            vim.command('silent cfile %s' % str2vimfmt(self.get_make_tmpfile()))
            vim.command('silent cd ' + str2vimfmt(cwd))

    def load_grep_result(self):
        if path.isfile(self.get_grep_tmpfile()):
            cwd = formpath(vim.eval('getcwd()'))
            grepprg = vim.eval("&grepprg")
            vim.command('silent cd ' + str2vimfmt(self.basedir))
            vim.command('silent set grepprg=' + str2vimfmt(r'''cat "%s"''' % self.get_grep_tmpfile()))
            vim.command('silent grep')
            vim.command('silent set grepprg=' + str2vimfmt(grepprg))
            vim.command('silent cd ' + str2vimfmt(cwd))

    def invert_warning(self):
        self.warning = not self.warning
        self.update_compiler_efm()
        self.load_make_result()

    def refresh_files(self):
        self.files = []
        for p in self.path:
            self.files += search_files(p.split('/'), self.suffix)
        with open(self.get_file_list(), 'w') as f:
            for fname in self.files:
                print >> f, fname

    def refresh_tags(self):
        call(['ctags','--c-kinds=+px', '--c++-kinds=+px', '--fields=+iaS', '--extra=+q', '-n', '-L', self.get_file_list(), '-f', self.get_tags_fname()])

    def refresh_cscope(self):
        if self.type in ['c', 'cpp', 'java']:
            vim.command('silent! cs kill -1')
            call(['cscope', '-b', '-k', '-f', self.get_cscope_fname(), '-i', self.get_file_list()])
            self.add_cscope_database()

    def update(self):
        self.refresh_files()
        self.refresh_tags()
        self.refresh_cscope()
        print "update over."

    def run_execute(self, args):
        if self.execute:
            execute = self.execute #.replace('/', '\\')
            origdir = formpath(vim.eval('getcwd()'))
            vim.command('silent! lcd ' + str2vimfmt(self.basedir))
            os.system(execute + ' ' + args + (' && pause || pause' if self.pause else ''))
            vim.command('silent! lcd ' + str2vimfmt(origdir))
        else:
            if vim.eval('&ft') in ['python', 'perl', 'lua']:
                os.system(vim.eval('&ft') + ' ' + vim.eval('''expand('%:p')''') + ' ' + args + ' && pause || pause')
            else:
                print >> sys.stderr, "no execute"


g_vimproject = VimProject()

def from_this_file():
    fname = vim.eval('''expand('%:p')''')
    g_vimproject.from_file(fname)

def update_project_history():
    fname = g_vimproject.projectfile
    iswin = int(vim.eval('g:isWin'))
    if fname.endswith(".vprj"):
        fs = []
        histfile = vim.eval("$HOME") + "/.vimproject"
        if path.isfile(histfile):
            fs = filter(lambda l:l, map(lambda l: l.strip(), open(histfile, "r").readlines()))
        try:
            if not iswin:
                ret = fs.index(fname)
                fs.pop(ret)
            else:
                for i in range(len(fs)):
                    if fs[i].lower() == fname.lower():
                        fs.pop(i)
                        break
        except:
            pass
        if len(fs) > 29:
            fs = fs[:29]
        fs.insert(0, fname)
        with open(histfile, 'w') as f:
            f.writelines(map(lambda l: l + '\n', fs))

def select_history_project():
    histfile = vim.eval("$HOME") + "/.vimproject"
    if path.isfile(histfile):
        fs = filter(lambda l:l, map(lambda l: l.strip(), open(histfile, "r").readlines()))
        if fs:
            ret = int(vim.eval('''inputlist(['Project history list here, select one:', %s])''' % ', '.join(map(lambda i: '"%2d: %s. %s"' % (i + 1, (path.basename(fs[i]) + ' ').ljust(30, '.'), path.dirname(fs[i])), range(len(fs))))))
            if 0 < ret <= len(fs):
                vim.command('silent edit %s' % str2vimfmt(fs[ret - 1]))
        else:
            print "no project history"
    else:
        print "no project history"


def edit_project_file():
    fname = g_vimproject.basedir + '/' + g_vimproject.projectname + '.vprj'
    if path.isfile(fname):
        vim.command('silent find %s' % str2vimfmt(fname))
    else:
        print >> sys.stderr, "Project file does not exist, cannot open it!"

def edit_file_list_file():
    fname = g_vimproject.get_file_list()
    if path.isfile(fname):
        vim.command('silent find %s' % str2vimfmt(fname))
    else:
        print >> sys.stderr, "File list file does not exist, cannot open it!"

def search_project_file():
    files = []
    cwd = formpath(vim.eval('getcwd()'))
    while 1:
        ret = os.listdir(cwd)
        for fname in ret:
            if fname.lower().endswith('.vprj'):
                files.append(cwd + '/' + fname)
        _cwd = path.dirname(cwd)
        if _cwd == cwd:
            break
        else:
            cwd = _cwd
    if files:
        ret = int(vim.eval('''inputlist(['Projects found, select one:', %s])''' % ', '.join(map(lambda i: '"%2d: %s. %s"' % (i+1, path.basename(files[i]) + ' '.ljust(30, '.'), path.dirname(files[i])), range(len(files))))))
        if 0 < ret <= len(files):
            vim.command('silent edit %s' % str2vimfmt(files[ret - 1]))
    else:
        print >> sys.stderr, "Have not found any project file."

def replace_pattern(pattern, repl):
    flist = g_vimproject.get_file_list()
    if path.isfile(flist):
        cwd = vim.eval('getcwd()')
        vim.command('silent cd ' + str2vimfmt(g_vimproject.basedir))
        os.system('cat "%s" | pyargs.py pyrep.py -i -b -f "%s" -t "%s"' % (flist, pattern, repl))
        vim.command('silent cd ' + str2vimfmt(cwd))
        vim.command('silent edit %')
        print "replace over."
    else:
        print >> sys.stderr, "%s not found!" % flist

def start_terminal_on_project():
    iswin = int(vim.eval('g:isWin'))
    if iswin:
        orig = os.getcwd()
        os.chdir(g_vimproject.basedir)
        #os.system('start cmd.exe')
        Popen("start Console.exe", shell = 1)
        os.chdir(orig)
    else:
        Popen("gnome-terminal --working-directory '%s'" % g_vimproject.basedir, shell = 1)

g_temp_path = ''
def before_quickfix_cmd():
    if g_vimproject.type != "python":
        global g_temp_path
        g_temp_path = formpath(vim.eval('getcwd()'))
        vim.command('silent lcd ' + str2vimfmt(g_vimproject.basedir))

def after_quickfix_cmd():
    if g_vimproject.type != "python":
        vim.command('silent lcd ' + str2vimfmt(g_temp_path))
    vim.command('cwindow')


def make_this_file():
    target = vim.eval("expand('%')")
    vim.command("silent make " + target)

PYTHON_EOF

au FileType vimproj py from_this_file()
au FileType vimproj py update_project_history()
au FileType vimproj set syntax=python
au BufWritePost *.vprj py from_this_file()

au QuickFixCmdPre * py before_quickfix_cmd()
au QuickFixCmdPost * py after_quickfix_cmd()

au VimLeavePre * py g_vimproject.write_session_file()

command -nargs=* Run py g_vimproject.run_execute('''<args>''')
command UpdateTags py g_vimproject.update()
command InvertWarning py g_vimproject.invert_warning()
command EditProject py edit_project_file()
command EditFileListFile py edit_file_list_file()
command SearchProject py search_project_file()
command SelectHistProject py select_history_project()
command LoadMakeResult py g_vimproject.load_make_result()
command LoadGrepResult py g_vimproject.load_grep_result()
command StartTerminal py start_terminal_on_project()
command LoadSessionFile py g_vimproject.load_session_file()
command MakeThisFile py make_this_file()

