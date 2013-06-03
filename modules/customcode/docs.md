Hosting your Custom Code on StackMob
==========

StackMob's Custom Code service and SDK enables you to implement your own server-side logic and still have access to all the same StackMob APIs for managing your app's data. We host and run your code so you never have to worry about configuring or managing your own servers.

## StackMob's Integration with GitHub

Using GitHub to link your Custom Code to StackMob allows you develop your code in a git repository and enables one click deployment of your code for quick iteration and a much easier experience than manually building and uploading JAR files.

# Getting Started on GitHub

To set up GitHub, you're going to need to download Git and configure your SSH keys. Follow <a href="https://help.github.com/articles/set-up-git" target="_blank">the directions on the GitHub site</a> to get up and running.  They've done a great job showing you how to get started.

You'll need to connect a GitHub Repository (repo) to your StackMob app. To get started implementing your own Custom Code methods you can fork the <a href="https://github.com/stackmob/stackmob-customcode-maven">stackmob-customcode-maven</a> repo, which has a simple pom.xml file pre-configured with the StackMob dependency and targets. Just add your own code and dependencies! You can read more about using the <a href="https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK">Custom Code SDK</a>.
If you don't fork the starter repo, it's important to note that your repo must:

  *  Contain a maven `pom.xml` file in the root directory.
  *  The pom must build a jar in the /target directory using the `package` lifecycle.
  *  The pom must contain the StackMob`customcode` dependency as seen in the example pom.

For you sbt fans, sbt support is on the way!

# Setting up GitHub with StackMob

Once you have a repo set up, it's time to link StackMob to your GitHub repository. Linking the two means we can build and deploy your code automatically when you push new code, rather than having to manually builda dn upload a JAR file yourself.

First, go to the <a href="https://dashboard.stackmob.com/module/customcode/view" target="_blank">Custom Code GitHub</a> page and click on "Link StackMob with GitHub".

A StackMob prompt will appear asking if you want to link StackMob with GitHub.  Accept the StackMob prompt's confirmation and you'll be redirect to GitHub site, where GitHub will ask permissions on its behalf.  Confirm there as well.

Upon successfully authenticating, you'll be shown your GitHub details:  your organization and your repositories.  Your organization is likely just your username.  Your repositories are the projects you've created on GitHub. Select your organization and the repo you created for your custom code and hit "Save".

Your StackMob account is now linked to GitHub!

# Deploying to StackMob via GitHub

As you continue to develop and commit your code to GitHub, you can either manually trigger StackMob to deploy your files or configure StackMob to automatically fetch files when you push to GitHub.

To manually refresh your site after a fresh commit to GitHub, go to the <a href="https://dashboard.stackmob.com/module/customcode/view" target="_blank">Custom Code GitHub</a> page and click on "Deploy Latest to Development". That's it!


Upon clicking, StackMob fetches your files from GitHub.  It takes a short while to deploy, however in a few seconds you'll be able to start using your updated API methods.


To make things even easier, you can set your application to automatically update the development site anytime time code is pushed to the master branch of the connected repo. To do this, copy the token listed under "Enable Automatic Updating From Github" on the <a href="https://dashboard.stackmob.com/module/customcode/view" target="_blank">Custom Code GitHub</a> page.

On the GitHub site, navigate to the front page of the repo you've connected to your app. Click on "Admin" to the right of the repo name and then "Service Hooks" on the Repository Administration page. Select "StackMob" and enter your token in the "Token" field and check "Active", then update the settings. Now every time you push to the repo, your development app will automatically be updated to serve the latest files.
