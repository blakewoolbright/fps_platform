if match( $TERM, "screen" ) != -1
  set term=xterm
endif

" Don't use Vi settings.
set nocompatible
set ignorecase
set smartcase

" Backspace should clear tabs, newlines, carriage returns.
set backspace=indent,eol,start
set bs=2

" Split vertically into equal divisions.
set splitright 

" Spaces per tab.
set shiftwidth=2
set tabstop=2

" Turn on file type detection.
filetype on

" Replace tabs with spaces
set expandtab

" Disable word wrap
set nowrap
set selectmode=mouse
set nobackup
set nowritebackup

" Always show cursor position.
set ruler

" Show certain whitespace characters.
set list
set listchars=tab:>.

" Autocommand - No autocomments for c/cpp
au FileType c,cpp setlocal comments-=:// comments+=f://
au FileType c,cpp setlocal comments-=:/*

" Enable syntax highlighing if possible.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" make it so lots of files get opened at once;
" this fixes the ^^ problem, where ^^ or :n would
" be disallowed because there were changes to the
" current file that weren't saved.
set hid

" --------------------------------
" COMMANDS
" --------------------------------i

set pastetoggle=<C-V>

" Always show cursor position.
set ruler
set wildignore=.svn,.git,.o,.a,.swp
set wildmenu

" Indent/Unindent
vnoremap < <gv
vnoremap > >gv

" Clear highlighted search by hitting return.
nnoremap <CR> :noh<CR><CR>

" To current file's directory.
command! CD cd %:p:h

" Remove trailing whitespace
command! Rmws %s/\s\+$// 

" Ctrl + Space toggles insert mode.
noremap  <C-@> <Esc>i
inoremap <C-@> <Esc>

" Ctrl+Z to 'Undo'
noremap <c-z> u
vnoremap <c-z> <Esc>uv
inoremap <c-z> <Esc>ui

" Ctrl+S to 'Save'
noremap <C-S> :w<CR>
vnoremap <C-S> <Esc>:w<CR>v
inoremap <C-S> <Esc>:w<CR>i

" Ctrl + B to 'open bash shell'
noremap  <C-B> :!bash<CR>
inoremap <C-B> <Esc>:!bash<CR>i
vnoremap <C-B> <Esc>:!bash<CR>v

" Ctrl+w to 'window view manip'
inoremap <C-w> <ESC>
vnoremap <C-w> <ESC>

" Disable selecting by page using Shift + Up/Down 
vnoremap <S-Down> j
vnoremap <S-Up> k

" Fat Finger Fixes
" cmap  W  w
" cmap Wq wq
" cmap  Q  q
command! W execute "write"
command! Q execute "quit"
command! Wq execute ":wq"
command! Qa execute ":qa"
command! B1 execute ":b1"
command! B2 execute ":b2"
command! B3 execute ":b3"
command! B4 execute ":b4"
command! B5 execute ":b5"
command! B6 execute ":b6"
command! B7 execute ":b7"
command! B8 execute ":b8"

autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" set t_Co=256
" colorscheme inkpot 
" colorscheme desert256

