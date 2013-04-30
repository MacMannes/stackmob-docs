# Hosting your HTML5 App on StackMob

StackMob's HTML5 service and <a href="https://developer.stackmob.com/tutorials/js">Javascript SDK</a> (built upon <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>) lets you host your web app on StackMob while connecting to StackMob's API seamlessly through same-domain Javascript AJAX calls.

## Why an HTML5 Hosting Service on StackMob?

Because your web app is served directly from StackMob, this circumvents any potential cross-domain AJAX call restrictions.  No matter where your web app is run, you don't need to worry about whether a browser supports CORS (cross-origin resource sharing) headers nor about same domain origin policies.  After all, it **is** the same domain.  In doing so, we broaden your app support so that you can reach a wider audience.

## StackMob's integration with GitHub

Using GitHub to link your files to StackMob allows you to have fine-grained control over what exactly you're releasing, while giving you the flexibility to quickly and <a href="https://github.com/features/hosting" target="_blank">easily edit your code</a> and <a href="https://github.com/features/projects" target="_blank">collaborate with other developers</a>.


# What is GitHub?

GitHub describes itself as: "... the best way to collaborate with others. Fork, send pull requests and manage all your public and private git repositories."  GitHub is an online service that provides version control for your code with Git.  Quoting GitHub again, "... an extremely fast, efficient, distributed version control system ideal for the collaborative development of software."

It's a great service that lets you develop your projects collaboratively, share and view code publicly or privately on the web, and it keeps track of all changes to your files as you progress.  We use it here daily at StackMob for our own projects.

If you aren't already on GitHub, you can <a href="https://github.com/signup/free" target="_blank">sign up for a free GitHub account</a>.  You can always upgrade to a paid account later if you'd like to have some private projects.


# Getting Started on GitHub

To set up GitHub, you're going to need to download Git and configure your SSH keys. Follow <a href="https://help.github.com/articles/set-up-git" target="_blank">the directions on the GitHub site</a> to get up and running.  They've done a great job showing you how to get started.

You'll need to connect a GitHub Repository (repo) to your StackMob app. If you'd like to use some example code to get up and running, our <a href="https://developer.stackmob.com/tutorials/js" target="_blank">Getting Started in HTML5</a> page will get you going. Otherwise, you'll want to <a href="https://help.github.com/articles/creating-a-new-repository" target="_blank">create a new repo</a> and push some HTML and/or JS files to it.

## Video: GitHub and StackMob Integration Tour

Here's a short video showing how a user like you can link your GitHub account to StackMob, and how StackMob can start seamlessly serving your web app for you automatically:

<p>
<iframe width="560" height="315" src="//www.youtube.com/embed/2KlW_ynQMDQ" frameborder="0" allowfullscreen="allowfullscreen"></iframe>
</p>

Read on to see how you can link your StackMob app with GitHub and start serving your web app from StackMob.

# Setting up GitHub with StackMob

Assuming you now have your web app files pushed to GitHub, it's time to link StackMob to your GitHub repository.  Linking the two simplifies your job of having to move files around because StackMob fetches your files from GitHub for you with a manual push of a button.  You can even configure it so that we do it automatically for you.

To serve the files in your repo through StackMob, visit our <a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank">Manage HTML5</a> page and click on "Link StackMob with GitHub".

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML5_Link_to_GitHub.png" /></a>
</div>

A StackMob prompt will appear (not pictured) asking if you want to link StackMob with GitHub.  Accept the StackMob prompt's confirmation and you'll be redirect to GitHub site, where GitHub will ask permissions on its behalf.  Confirm there as well.

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_authorizegithub.jpg" /></a>
</div>

Upon successfully authenticating, you'll be shown your GitHub details:  your organization and your repositories.  Your organization is likely just your username.  Your repositories are the projects you've created on GitHub. Select your organization and the repo you created for your web app and hit "Save".

<div>
<a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_select_repo.jpg" /></a>
</div>

Your StackMob account is now linked to GitHub! StackMob will fetch your files and start serving them in your development environment for you immediately.  To confirm this, you can go to the live URL listed below your repo on the settings page.

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_liveURL.jpg" /></a>
</div>

Below, presume that we have the file `index.html` in the root directory of our repo declaring `Hello World!`.

<div class="screenshot"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_helloworld1.png" />
</div>

If at any time you want to stop hosting files on StackMob, you can disable hosting by unchecking the "Enable HTML5 Serving".  The URL will then no longer be active.

# Deploying to StackMob via GitHub

As you continue to develop and commit your code to GitHub, you can either manually trigger StackMob to deploy your files or configure StackMob to automatically fetch files when you push to GitHub.

To manually refresh your site after a fresh commit to GitHub, go to the <a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank">Manage HTML5</a> page and click on "Refresh from GitHub". That's it!

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_manualdeploy.jpg" style="width:261px"/></a>
</div>

Upon clicking, StackMob fetches your files from GitHub.  It takes a short while to deploy, however in a few seconds you'll see your updates in their full glory:

<div class="screenshot"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_helloworld2.jpg" />
</div>

To make things even easier, you can set your application to automatically update the development site anytime time code is pushed to the connected repo. To do this, copy the token listed under "Enable Automatic Updating From Github" on the <a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank">Manage HTML5</a> page.

On the GitHub site, navigate to the front page of the repo you've connected to your app. Click on "Admin" to the right of the repo name and then "Service Hooks" on the Repository Administration page. Select "StackMob" and enter your token in the "Token" field and check "Active", then update the settings. Now every time you push to the repo, your development app will automatically be updated to serve the latest files.

<div class="screenshot"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_servicehooks.jpg" />
</div>
