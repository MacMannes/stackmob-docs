# Deploying your HTML5 Web App to Production

This guide walks you through the step of deploying your HTML5 from your Development environment to a Production environment.

StackMob gives you two HTML5 environments so that you can test your HTML on the web: Development and Production.  In these cases you will need to deploy your backend component - your API, and you will need to deploy your frontend component - your HTML web assets.  This guide focuses on deploying your HTML web assets.

## Basics: Development and Production HTML5 environments

By default, we provide you with stackmob-hosted URLs constructed with your application name and subdomain (which we auto-generate from your name or company name):

		//Development URL
		http://dev.appname.subdomain.stackmobapp.com
		
		//Production URL
		http://appname.subdomain.stackmobapp.com
		
The Development URL is for you to freely rollout changes while you're developing and tweaking your UI or adding new features.  It's where you can deploy your HTML to see how it will behave in a "live" environment.  The Production URL is to be more stable - used as your publicly facing URL that your users would use.

# Deployment Steps

There are generally three steps to deploy:

1.  Deploy your API (backend)
2.  Update your StackMob.init(...) Javascript to point your StackMob API calls to production servers.
3.  Deploy your HTML5 web assets (frontend) 

## 1 of 3: Deploying your API

Before deploying your HTML5 app to production, be sure to <a href="https://developer.stackmob.com/tutorials/dashboard/Deploying-your-StackMob-App" target="_blank">deploy a Production version of your API</a>, your backend component.  After all, without a production version of your API, your production HTML5 will have nothing to interact with!

## 2 of 3: Updating StackMob.init(..) to point to production

Update your StackMob.init(..) to point to production by setting it to `apiVersion: 1`.  

* If you're using OAuth 2.0 and setting your `public key` don't forget to update it to your `Production` public key as well.

**StackMob JS SDK 0.5.0+**

```javascript
StackMob.init({
	appName: 'yourapp',
	clientSubdomain: 'yoursubdomain',
	publicKey: 'yourproductionpublickey',
	apiVersion: 1 
});
```

**StackMob JS SDK 0.1.1+**

```javascript
StackMob.init({
	appName: 'yourapp',
	clientSubdomain: 'yoursubdomain',
	apiVersion: 1 
});
```

**StackMob JS SDK 0.1.0**

```javascript
StackMob.init({
	appName: 'yourapp',
	apiVersion: 1,
	dev: false 
});
```


## 3 of 3: Deploying your Web Assets

After deploying your API to production, you can roll out your web assets to your production URL.  When you deploy, StackMob deploys what you have in your development environment to production.  So first make sure that what you see at dev.yourapp.yoursubdomain.stackmobapp.com is what you're looking to deploy.  Next:

1.  <a href="https://dashboard.stackmob.com/deploy" target="_blank">Visit the Deploy Page</a>
2.  Check off the HTML5 box
3.  Review the information - a copy of your development web assets will be rolled to production
4.  Deploy

After clicking deploy, your web assets will be deploying to your production URL in short time, depending on the size of your web assets.  After rollout, your production URL will have a copy of your web assets in development.  (Meanwhile, any future web asset changes in your development environment will not affect production - until you deploy again.)

(Example production url:  http://yourapp.yourdomain.stackmobapp.com/ )

# Deployment History

You can <a href="https://dashboard.stackmob.com/module/html5/deployment_history" target="_blank"> review your HTML5 deployment history</a> to see what you've deployed to development and production.  (Development deployments are when you manually refresh the content from GitHub or - if you've setup GitHub deployment hooks - whenever you commit to GitHub.)

