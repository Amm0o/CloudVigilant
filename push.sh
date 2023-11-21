# clean compiled version
make clean

# ask for commit message
echo "Enter commit message: "
read commitMessage

# commit and push
git add -A
git commit -a -m "$commitMessage"
git push