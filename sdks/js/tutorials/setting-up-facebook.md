<h3>Objective</h3>
Setting up Facebook SDK

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~10 minutes

<h3>Prerequisites</h3>

* Have a Facebook account

<h1>Let's get started!</h1>

<h2>Setting up Facebook App</h2>

<h3>Hosts File</h3>

In this tutorial, we will use `developer.localhost` as our example. First, you will need to modify your `hosts` file and add `127.0.0.1  developer.localhost`. If you don't know the location of your `hosts` file, try to look it up on <a href="http://en.wikipedia.org/wiki/Hosts_file" target="_blank">Wikipedia</a>. Remember to use a `tab` space between `127.0.0.1` and `developer.localhost`.

<h3>Setting Up Facebook App</h3>

<a href="https://developers.facebook.com/docs/guides/mobile/web/">Create a Facebook App</a>. Follow step 1 and 2 in Getting Started.

For the App Domains, set it to `developer.localhost`.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/facebook_app_domains.png"></img>

Below there, setup the `Website with Facebook Login` and let's just put `http://developer.localhost:4567` for the `Site URL`

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/facebook_website_url.png"></img>

Click `Save Changes` and do not close the window since we need to use your Facebook `App ID`.

<h3>Setting up Facebook SDK</h3>

For simplicity, let's say that we have `index.html` in the same directory as `stackmobserver.py` file.

Since we set the domain to `developer.localhost` for App Domains in our Facebook app, remember to access the app through `http://developer.localhost:4567` instead of `http://localhost:4567/` or `http://127.0.0.1:4567/`.

Your Facebook JS initialization should look similar like this:

```js,5,6
//What follows is standard Facebook JS initialization.  See Facebook for more details.
//Facebook JS SDK code here.  Use your own Facebook App ID
window.fbAsyncInit = function() {
  FB.init({
    appId      : 'your_FB_app_ID', // FIXME
    channelUrl : '//developer.localhost:4567/index.html', // Channel File
    status     : true, // check login status
    cookie     : true, // enable cookies to allow the server to access the session
    xfbml      : true  // parse XFBML
  });

  // Additional initialization code here
};

// Load the SDK Asynchronously
(function(d){
   var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
   js = d.createElement('script'); js.id = id; js.async = true;
   js.src = '//connect.facebook.net/en_US/all.js';
   d.getElementsByTagName('head')[0].appendChild(js);
 }(document));
```