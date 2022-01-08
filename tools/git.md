# 合并其他分支内容到自己分支
> git fetch   
git merge origin/ofs4.0.x_common you_branch   
git push   
# 统计代码修改量
> git log --since="2021-07-01" --before="2021-12-30" --author="limingming" --pretty=tformat: --numstat | gawk '{ add += $1 ; subs += $2 ; loc += $1 - $2 } END { printf "added lines: %s removed lines: %s total lines: %s\n",add,subs,loc}'   
