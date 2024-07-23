# clean compiled version
make clean

# ask for commit message
echo "Enter commit message: "
read commitMessage

# commit and push
git add -A
git commit -a -m "$commitMessage"
git push https://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@github.com/Amm0o/Heimdall.git