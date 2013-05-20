<h1>StackMob Environments</h1>

# What are Development and Production Environments?

In order to effectively develop your mobile application, you need a safe area to make changes while your production environment stays stable.  StackMob provides you with a Development and Production environment so that you can do so.  You configure the StackMob SDKs to point to your Development or Production environments.  Underneath the covers, the SDKs will set a Request Header specifying which API to hit.

## Development Environment

When you first signup with StackMob, you're taken to your Development environment.  Schemas you create there, custom code you write, and the data you save will all be for your Development environment.  You can make changes to schemas and update your custom code without worrying about how it will affect users in production.

Your Development REST API is located at:

    http://api.stackmob.com/

    With request header:
    Accept: application/vnd.stackmob+json; version=0      

Note the 0, for API Version 0 - development

You should be passing your [Development public key](https://dashboard.stackmob.com/settings) with your REST API request.  The StackMob iOS and Android SDKs will do that for you.  If you're formulating your own REST API requests, be sure to include your public key in your request as according to the 2-legged OAuth spec.

## Production Environment

Your Production environment serves your public facing traffic.  It's a completely separate environment from Development.  Your app data in the Production environment stays in Production, and changes to schemas, custom code, and data in Development do not affect the Production environment.  This lets you experiment and develop safely while you serve your live traffic.  The Production environment does not allow changes to schemas or custom code outside of StackMob's deployment process.  (Read more about deployment below).

Your Production REST API, after you deploy it, is located at:

    http://api.stackmob.com/

    With request header:
    Accept: application/vnd.stackmob+json; version=1

You should be passing your [Production public key](https://dashboard.stackmob.com/settings) with your REST API request.

### Multiple Production APIs

Upgrading your app and backend could be a challenging endeavor given that you can't force your users to upgrade to the latest version.  If you add new schemas or change your custom REST API endpoints in your latest iteration of your app, this would normally break backwards compatibility with your old users.  This is where multiple Production API versions come in.

StackMob provides the ability for you to run multiple Production API versions.  If you had three separate API versions running at the same time, you'd have Production REST API endpoints such as:

    http://api.stackmob.com/
    
    With request headers:
    Accept: application/vnd.stackmob+json; version=1  //Hits API Version 1
    Accept: application/vnd.stackmob+json; version=2  //Hits API Version 2
    Accept: application/vnd.stackmob+json; version=3  //Hits API Version 3

Each one of those REST API versions could be concurrently running with a different set of schemas and custom code.  They would all be sharing the same data source.  This allows you to upgrade your backend and potentially change the interface while giving you the flexibility you need to make decisions on how to support backwards compatibility with your users.  For example, did you refactor a major server-side component in Custom Code and want to safely section off new users to use that logic?  Deploy that logic to Version 2 of your API and rollout a version of your mobile client to use that API.  Meanwhile, users with your original app can still hit Version 1 of your API.

Here's an example of the administrative page for your Live APIs.

<p class="screenshot">
<a href="https://dashboard.stackmob.com/module/apiversions/view"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_Deploy_Live.png" alt=""/></a>
</p>


### Handling Changes to Schemas

In between your API Versions, you may modify fields on your schemas.  As with any application, let alone StackMob powered ones, changing a schema needs to be thought out completely.  Objects are saved as JSON objects in StackMob's datastores, so adding or removing fields from your schemas will eventually result in older objects missing newer fields or newer objects missing older fields.  You application should take this into consideration.

# How do I Deploy to Production?

Deployment happens through StackMob's UI Platform via the [Deploy page](https://dashboard.stackmob.com/deploy).  Deploying is the process of taking whatever state your schemas and custom code are in Development and then rolling

Each time you deploy your changes, StackMob quickly and easily lets you revision and deploy your API as a particular version.

Here's an example of the deploy page.  Select what API version you want to deploy to, enter a description, and deploy!

<p class="screenshot">
<a href="https://dashboard.stackmob.com/deploy"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_Update_API.png" alt=""/></a>
</p>


# How to deploy Multiple API Versions in Production

Deploying to Multiple API Versions is as easy as deploying via the [Deploy page](https://dashboard.stackmob.com/deploy) and selecting a different version number.  That's it!  On your mobile app, you would change your StackMob configuration to the appropriate version number.

* [Configure your iOS APP API Version](https://dashboard.stackmob.com/stackmob-ios-sdk/configure)
* [Configure your Android App API Version](https://developer.stackmob.com/stackmob-android-sdk/configure)

Simply select API Version 2, 3, 4, and so on when deploying to run more concurrent APIs.


<p class="screenshot">
<a href="https://dashboard.stackmob.com/deploy"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_Deploy_Concurrent_API.png" alt=""/></a>
</p>
