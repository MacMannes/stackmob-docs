Facebook and JavaScript
====================

## Setup

### Prerequisites

* Have a Facebook account

### Hosts File

In this tutorial, we will use `developer.localhost` as our example. First, you will need to modify your `hosts` file and add `127.0.0.1  developer.localhost`. If you don't know the location of your `hosts` file, try to look it up on <a href="http://en.wikipedia.org/wiki/Hosts_file" target="_blank">Wikipedia</a>. Remember to use a `tab` space between `127.0.0.1` and `developer.localhost`.

### Setting Up Facebook App

<a href="https://developers.facebook.com/docs/guides/mobile/web/">Create a Facebook App</a>. Follow step 1 and 2 in Getting Started.

For the App Domains, set it to `developer.localhost`.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/facebook_app_domains.png"></img>

Below there, setup the `Website with Facebook Login` and let's just put `http://developer.localhost:4567` for the `Site URL`

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/facebook_website_url.png"></img>

Click `Save Changes` and do not close the window since we need to use your Facebook `App ID`.

### Setting up Facebook SDK

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



## Create a user with Facebook

Have this code below somewhere in your body tag after you initialize Facebook SDK

```js
<!-- You can also add `class="fb-login-button"` to make it look like a Facebook button -->
<a href="javascript:void(0);" id="login">Log into Facebook</a>

<div id="fb-root"></div>

<!-- Best to put this JS before the end of your body tag, i.e. before </body> -->
<script type="text/javascript">
  function login() {
    //Login with Facebook
    FB.login(function(response) {
      if (response.authResponse) {
        var accessToken = response.authResponse.accessToken;

        FB.api('/me', function(response) {
          var user = new StackMob.User({ username: response.email });
          user.createUserWithFacebook(accessToken, {
            success: function(model) {
              console.debug(model);
            },
            error: function(model, response) {
              console.debug(model);
              console.debug(response);
            }
          });
        });

      } else {
        console.log('User cancelled login or did not fully authorize.');
      }
    }, {scope: 'email'});
  }

  $('#login').click(function() {
    login();
  });
</script>
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-create" target="_blank">create</a></li>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-createuserwithfacebook" target="_blank">createUserWithFacebook</a></li>
      </ul>
    </div>
  </div>
</div>


## Login with Facebook

Have this code below somewhere in your body tag after you initialize Facebook SDK.

```js
<!-- You can also add `class="fb-login-button"` to make it look like a Facebook button -->
<a href="javascript:void(0);" id="login">Login with FB Token</a>

<div id="fb-root"></div>

<!-- Best to put this JS before the end of your body tag, i.e. before </body> -->
<script type="text/javascript">
  // Feel free to change the `username` and `password` if you want to link other `User`
  var userToLink = new StackMob.User({ username: 'tolink', password: 'mypassword' });

  function login() {
    //Login with Facebook
    FB.login(function(response) {
      if (response.authResponse) {
        var accessToken = response.authResponse.accessToken;
        var user = new StackMob.User();
        user.loginWithFacebookToken(accessToken, false, {
          success: function(model) {
            console.debug(model);
          },
          error: function(model, response) {
            console.debug(model);
            console.debug(response);
          }
        });
      } else {
        console.log('User cancelled login or did not fully authorize.');
      }
    }, {scope: 'email'});
  }

  $('#login').click(function() {
   login();
  });
</script>
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-loginwithfacebooktoken" target="_blank">loginWithFacebookToken</a></li>
      </ul>
    </div>
  </div>
</div>



## Link user with Facebook

In this tutorial, we will assume that you have a `User` with `username: tolink` and `password: mypassword`.

Have this code below somewhere in your body tag after you initialize Facebook SDK.

```js
<!-- You can also add `class="fb-login-button"` to make it look like a Facebook button -->
<a href="javascript:void(0);" id="link-user">Link User with Facebook</a>

<div id="fb-root"></div>

<!-- Best to put this JS before the end of your body tag, i.e. before </body> -->
<script type="text/javascript">
  // Feel free to change the `username` and `password` if you want to link other `User`
  var userToLink = new StackMob.User({ username: 'tolink', password: 'mypassword' });

  function login() {
    //Login with Facebook
    FB.login(function(response) {
      if (response.authResponse) {
        var accessToken = response.authResponse.accessToken;

        FB.api('/me', function(response) {
          userToLink.linkUserWithFacebook(accessToken, {
            success: function(model) {
              console.debug(model);
            },
            error: function(model, response) {
              console.debug(model);
              console.debug(response);
            }
          });
        });

      } else {
        console.log('User cancelled login or did not fully authorize.');
      }
    }, {scope: 'email'});
  }

  userToLink.login(false, {
    success: function(model) {
      console.debug(model);
      $('#link-user').click(function() {
        login();
      });
    },
    error: function(model, response) {
      console.debug(model);
      console.debug(response);
      $('#link-user').click(function() {
        alert("Something went wrong when trying to log " + userToLink.toJSON().username + " in. Please check your web browser console");
      });
    }
  });
</script>
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-create" target="_blank">create</a></li>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-login" target="_blank">login</a></li>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-linkuserwithfacebook" target="_blank">linkUserWithFacebook</a></li>
        <li><a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-tojson" target="_blank">toJSON</a></li>
      </ul>
    </div>
  </div>
</div>
