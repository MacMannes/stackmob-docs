StackMob HTML Hosting
=====================================

## Hosting your HTML5 App on StackMob

StackMob's HTML5 service and <a href="https://developer.stackmob.com/tutorials/js">Javascript SDK</a> (built upon <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>) lets you host your web app on StackMob while connecting to StackMob's API seamlessly through same-domain Javascript AJAX calls.

### Why an HTML5 Hosting Service on StackMob?

Because your web app is served directly from StackMob, this circumvents any potential cross-domain AJAX call restrictions.  No matter where your web app is run, you don't need to worry about whether a browser supports CORS (cross-origin resource sharing) headers nor about same domain origin policies.  After all, it **is** the same domain.  In doing so, we broaden your app support so that you can reach a wider audience.

## StackMob's integration with GitHub

Using GitHub to link your files to StackMob allows you to have fine-grained control over what exactly you're releasing, while giving you the flexibility to quickly and <a href="https://github.com/features/hosting" target="_blank">easily edit your code</a> and <a href="https://github.com/features/projects" target="_blank">collaborate with other developers</a>.


### What is GitHub?

GitHub's a version control service based on Git. It's easy to use and read to go for you immediately.  If you aren't already on GitHub, you can <a href="https://github.com/signup/free" target="_blank">sign up for a free GitHub account</a>.  You can always upgrade to a paid account later if you'd like to have some private projects.

It's great to save your code there and collaborate with team members and can greatly help.


### Video: GitHub and StackMob Integration Tour

Here's a short video showing how a user like you can link your GitHub account to StackMob, and how StackMob can start seamlessly serving your web app for you automatically:

<p>
<iframe src="http://player.vimeo.com/video/69333500" width="500" height="281" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>

<!--
<iframe width="560" height="315" src="//www.youtube.com/embed/2KlW_ynQMDQ" frameborder="0" allowfullscreen="allowfullscreen"></iframe>
-->
</p>




Read on to see how you can link your StackMob app with GitHub and start serving your web app from StackMob.

## Setting up GitHub with StackMob

Let's link your existing GitHub repository with StackMob.  Visit your <a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank">HTML5 Hosting settings page</a>.

Click "Link StackMob with GitHub" and accept the authorization request.  You'll see a UI where you can choose your GitHub organization and Repository.

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-linked-account.png" alt=""/></a></p>

Choose your repository and StackMob will fetch your files and start serving them in your development environment for you immediately.  You can check its status:

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-status.png" /></a>
</p>

And you can also check your roll history:

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/html5/deployment_history" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-roll-history.png" /></a></p>

Below, presume that we have the file `index.html` in the root directory of our repo declaring `Hello World!`.

<p class="screenshot"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_helloworld1.png" /></p>

If at any time you want to stop hosting files on StackMob, you can disable hosting by unchecking the "Enable HTML5 Serving".  The URL will then no longer be active.

## Enabling automatic deployment

StackMob can also automatically deploy your development environment whenever you commit to your GitHub repository.  Just enable that functionality below:

<div class="screenshot"><a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-service-hook.png" /></a>
</div>

## New to GitHub?

Of course you'll need to <a href="https://github.com/signup/free" target="_blank">sign up for a free GitHub account</a>.

To set up GitHub, you're going to need to download Git and configure your SSH keys. Follow <a href="https://help.github.com/articles/set-up-git" target="_blank">the directions on the GitHub site</a> to get up and running.  They've done a great job showing you how to get started.

Once signed up, you'll want to <a href="https://help.github.com/articles/creating-a-new-repository" target="_blank">create a new repo</a> and push some HTML and/or JS files to it, then link your repository to StackMob!
