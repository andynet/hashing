testing uses check

```bash
sudo apt-get install check
```

# How to install criterion
download binary from https://github.com/Snaipe/Criterion/releases/tag/v2.3.3 \
unpack with `tar -xvjf criterion-v2.3.3-linux-x86_64.tar.bz2` \
then
```
sudo mv include/* /usr/local/include/
sudo mv lib/*     /usr/local/lib/
sudo mv share/*   /usr/local/share/
ldconfig /usr/local/lib/
```