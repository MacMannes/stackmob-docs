<h3>Objective</h3>
Create a new `user` object with Facebook

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~8 minutes

<h3>Prerequisites</h3>

* An application in StackMob

* <a href="https://developer.stackmob.com/tutorials/js/Setting-Up-Facebook" target="_blank">Correct initialization of Facebook SDK</a>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* A StackMob `User` to link (add using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create a User object tutorial</a>)

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-create" target="_blank">create</a>

* <a href="https://developer.stackmob.com/sdks/js/api#a-login" target="_blank">login</a>

* <a href="https://developer.stackmob.com/sdks/js/api#a-linkuserwithfacebook" target="_blank">linkUserWithFacebook</a>

* EXTRA: <a href="https://developer.stackmob.com/sdks/js/api#a-tojson" target="_blank">toJSON</a>

<h2>Link User with Facebook</h2>

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