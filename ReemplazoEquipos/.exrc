let s:cpo_save=&cpo
set cpo&vim
inoremap <silent> <expr> <Plug>_ deoplete#mapping#_dummy_complete()
inoremap <silent> <SNR>27_AutoPairsReturn =AutoPairsReturn()
inoremap <silent> <expr> <Plug>(neosnippet_start_unite_snippet) unite#sources#neosnippet#start_complete()
inoremap <silent> <expr> <Plug>(neosnippet_jump) neosnippet#mappings#jump_impl()
inoremap <silent> <expr> <Plug>(neosnippet_expand) neosnippet#mappings#expand_impl()
inoremap <silent> <expr> <Plug>(neosnippet_jump_or_expand) neosnippet#mappings#jump_or_expand_impl()
inoremap <silent> <expr> <Plug>(neosnippet_expand_or_jump) neosnippet#mappings#expand_or_jump_impl()
inoremap <C-Space> /[\]\>})'"]:nohla
snoremap  a<BS>
nnoremap  
nnoremap <NL> <NL>
nnoremap  
nnoremap  
snoremap  a<BS>
noremap  A;
tnoremap  
noremap  l :nohl
noremap  q :q
noremap  w :wa
noremap  t :vs term://zsh:vertical resize 50
noremap  s :FZF
nnoremap ,w =
nnoremap ,f _|
tnoremap ;; 
nnoremap H gT
nnoremap L gt
xmap [% [%m'gv``
xmap ]% ]%m'gv``
xmap a% [%v]%
xmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
snoremap <Del> a<BS>
snoremap <BS> a<BS>
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
xnoremap <silent> <Plug>(neosnippet_register_oneshot_snippet) :call neosnippet#mappings#_register_oneshot_snippet()
xnoremap <silent> <Plug>(neosnippet_expand_target) :call neosnippet#mappings#_expand_target()
xnoremap <silent> <Plug>(neosnippet_get_selected_text) :call neosnippet#helpers#get_selected_text(visualmode(), 1)
snoremap <silent> <expr> <Plug>(neosnippet_jump) neosnippet#mappings#jump_impl()
snoremap <silent> <expr> <Plug>(neosnippet_expand) neosnippet#mappings#expand_impl()
snoremap <silent> <expr> <Plug>(neosnippet_jump_or_expand) neosnippet#mappings#jump_or_expand_impl()
snoremap <silent> <expr> <Plug>(neosnippet_expand_or_jump) neosnippet#mappings#expand_or_jump_impl()
noremap <C-Space> /[\]\>})'"]:nohl
tnoremap <S-CR> 
inoremap <expr>  pumvisible() ? "\" : "\u\"
inoremap  A;
cmap w!! w !sudo tee > /dev/null %
let &cpo=s:cpo_save
unlet s:cpo_save
set background=dark
set clipboard=unnamedplus
set expandtab
set fillchars=vert:│
set helplang=en
set runtimepath=~/.config/nvim,~/.fzf/,~/.local/share/nvim/plugged/vim-airline/,~/.local/share/nvim/plugged/vim-airline-themes/,~/.local/share/nvim/plugged/deoplete.nvim/,~/.local/share/nvim/plugged/neosnippet.vim/,~/.local/share/nvim/plugged/neosnippet-snippets/,~/.local/share/nvim/plugged/auto-pairs/,~/.local/share/nvim/plugged/gruvbox/,~/.local/share/nvim/plugged/vim-pandoc/,~/.local/share/nvim/plugged/vim-pandoc-syntax/,~/.local/share/nvim/plugged/haskell-vim/,~/.local/share/nvim/plugged/vimtex/,~/.local/share/nvim/plugged/indentLine/,/etc/xdg/nvim,~/.local/share/nvim/site,/usr/local/share/nvim/site,/usr/share/nvim/site,/usr/share/nvim/runtime,/usr/share/nvim/site/after,/usr/local/share/nvim/site/after,~/.local/share/nvim/site/after,/etc/xdg/nvim/after,~/.config/nvim/after,~/.local/share/nvim/plugged/haskell-vim/after,~/.local/share/nvim/plugged/vimtex/after,~/.local/share/nvim/plugged/indentLine/after,/usr/share/vim/vimfiles
set shiftwidth=0
set splitbelow
set splitright
set tabstop=2
set window=38
" vim: set ft=vim :
