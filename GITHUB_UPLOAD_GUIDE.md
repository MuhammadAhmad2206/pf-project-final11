# GitHub Upload Guide

## Option 1: Upload from GitHub Website

1. Create a new repository.
2. Click **Add file**.
3. Click **Upload files**.
4. Upload all project files.
5. Commit changes.

Note: This method creates only one commit.

## Option 2: Recommended Git Method

Open terminal inside the project folder and run:

```bash
git init
git add .
git commit -m "Create initial PF project structure"
git branch -M main
git remote add origin YOUR_REPOSITORY_LINK
git push -u origin main
```

For better marks, commit module by module using the plan given in README.md.
