<h3>Objective</h3>
Create a new `user` object with Facebook

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~8 minutes

<h3>Prerequisites</h3>

* An application in StackMob

* <a href="https://developer.stackmob.com/tutorials/js/Setting-Up-Facebook" target="_blank">Correct initialization of Facebook SDK</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-create" target="_blank">create</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-createuserwithfacebook" target="_blank">createUserWithFacebook</a>

<h2>Create User with Facebook</h2>

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