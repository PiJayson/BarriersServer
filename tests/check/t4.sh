while read line ; do echo $line | xargs -n1 | sort | xargs  ; done | sort | uniq | wc -l
