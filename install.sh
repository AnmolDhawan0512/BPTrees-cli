g++ b+tree.cpp -o btrees
mkdir ~/bin
ln -s "$(pwd)/btrees" ~/bin/btrees
echo 'export PATH="$PATH:~/bin"' >> ~/.zshrc
echo 'export PATH="$PATH:~/bin"' >> ~/.bash_profile
