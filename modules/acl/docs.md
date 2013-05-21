Access Controls - Schema Permissions
=====================================

StackMob provides a way for you to set access level controls on your schemas.  You can limit your users' Create, Read, Update, or Delete access with various permission levels.  This feature is implemented using OAuth 2 and supported by the current versions of our all our SDKs.

You can take advantage of access controls by changing your schema settings through <a href="https://dashboard.stackmob.com/schemas/" target="_blank">Editing your Schemas</a> on the web platform.  Access Control permissions are at the bottom of the page.

Example screenshot of Editing the User Schema:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-01.png" alt=""/></a>
</p>

* Access Controls do not affect the Server-Side Custom Code feature StackMob offers.



# Why OAuth 2.0?

More and more apps are being written in Javascript, connecting to remote systems with AJAX. How do we know that the user making the call is allowed to do so? We can't hide secret keys in the source since Javascript source is out in the open. This is where OAuth 2.0 swoops in.

OAuth 2.0 is a username/password based security protocol where upon successfully logging in, a user is given a secure access token that he/she uses to make calls for the session. That access token is used to generate a one-time-use, encrypted, time-sensitive authorization signature each time your user makes an API call. Because the security is based on username/passwords, viewing the source won't expose any critical keys.

If you've logged into websites with Facebook or Twitter's JS SDKs, you've used OAuth 2.0! They've chosen OAuth 2.0 to secure their app for the same reasons - to protect their viewable-source SDKs while not exposing any private keys. So, continue developing knowing you're backed by industry-standard security through StackMob.

**OAuth 2.0 is supported in the following StackMob SDKs: iOS SDK v0.5.0+, Android SDK v0.5.0+, JS SDK v0.5.0+**


# Access Levels

The following access levels are currently available in order of most restrictive to least.  You can assign these individually to each action (Create, Read, Update, Delete).

* <a href="#a-not_allowed">Not Allowed</a>
* <a href="#a-allowed_with_private_key">Allowed with Private Key</a>
* <a href="#a-allowed_when_logged-in">Allowed when Logged-in</a>
  * <a href="#a-allow_to_object_owner">Allow to Object Owner</a>
  * <a href="#a-users_defined_in_a_role">Users defined in a Role</a>
  * <a href="#a-any_user_with_a_relationship_to_the_object">Any User with a relationship to the object</a>
  * <a href="#a-allow_to_any_logged_in_user">Allow to Any Logged In User</a>
* <a href="#a-open__accessible_with_public_key_or_private_key_">Open</a>


Let's see what these do.

## Not Allowed

At this permission level, don't allow anybody to access this schema through the REST API. 

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-02.png" alt=""/></a>
</p>

For instance, you can set `Create, Update, and Delete` to `Not Allowed` but allow Read access to `Open`, thereby limiting your schema to be read-only to anybody.

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-03.png" alt=""/></a>
</p>


`Not Allowed` is available to the following SDKs:

* iOS - all versions
* Android - all versions
* JavaScript - all versions


## Allowed with Private Key

At this permission level, as long as the API request is signed with the public and private key combo (OAuth 1.0), you can manipulate the data in your schemas. (Only applicable to StackMob SDKs *below* v0.5.0, as versions above v0.5.0 use OAuth 2.0, which does *not* use the app's private key.)

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-04.png" alt=""/></a>
</p>



StackMob generates <a href="https://dashboard.stackmob.com/settings" target="_blank">Public and Private keys</a> for your application.  The iOS v0.4.12, Android v0.4.7 and JavaScript v0.3.0 and earlier SDKs use the public and private key combos to sign each of your API requests.

You should **never** give anybody else your private key!  These are treated as super-admin level identifiers.

With OAuth 2.0 you no longer pass the private key.  In effect, the `Allowed with Private Key` level acts like `Not Allowed` under OAuth 2.

* The <a href="https://dashboard.stackmob.com/data/console/" target="_blank">Test Console</a> and <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> make calls to your API using public/private keys.

The `Allowed with Private Key` access level is available to the following SDKs:

* iOS v0.4.12 and below
* Android v 0.4.7 and below
* Javascript v0.3.0 and below


## Logged In Permissions

*The following permissions only work with OAuth 2.0 versions of our SDKs:*

* iOS SDK v0.5.0+
* Android v0.5.0+
* JS SDK v0.5.0+

Also, if you are using an older SDK (before version 0.5.0), then your app is using OAuth 1.0 and will trump these settings, giving access.

There are various levels of access for users who've logged-in.  These are generally listed from most to least restrictive.

* <a href="#a-allow_to_object_owner">Allow to Object Owner</a>
* <a href="#a-any_user_with_a_relationship_to_the_object">Any User with a relationship to the object</a>
* <a href="#a-related_users_on_the_sm_owner_instance">Related Users on the sm_owner instance</a>
* <a href="#a-users_defined_in_a_role">Users defined in a Role</a>
* <a href="#a-allow_to_any_logged_in_user">Allow to Any Logged In User</a>


### Allow to Object Owner

At this permission level, only allow the logged in owner of the object instance to perform the action on the object.  For instance, if user `johndoe` created a new `todo` record, and the `todo` schema had `Read` set to this permission level, then only `johndoe` would be allowed to read this  `todo` record.

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-05.png" alt=""/></a>
</p>

This is facilitated via a StackMob-auto-generated field named `sm_owner`.  If your schema currently doesn't have that field, StackMob will add it on the fly upon API request.  StackMob will auto-populate this with the logged-in app user.  If there is no logged in app user at the time of object creation, `sm_owner` will not be set.

Here's an example of a `todo` object created by `johndoe` in JSON format:

```javascript
{
   todo_id: '12345',
   title: 'Take out the trash',
   sm_owner: 'user/johndoe', //auto filled by StackMob
   createddate: ...,
   lastmoddate: ...
}
```

Only `johndoe` would be able to access `todo` of id `12345` when logged in.



*Your users must be logged in and the owner of the object in order to take advantage of this access level.*

Available to the following SDKs:

* iOS SDK v0.5.0+
* Android v0.5.0+
* JS SDK v0.5.0+


### Any User with a relationship to the object

Only users defined in a user relationship to that object instance can perform the action (CREATE, READ, UPDATE or DELETE) on that object.

Here's an example of the `todo` schema's READ permissions being set to `Allow to collaborators Relationship`.  Note that there is a relationship of one to many users named `collaborators` on the schema.

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-06.png" alt=""/></a>
</p>

The following todo object can only be read by `julie`, `bob`, or `mike` because the READ permission is set to `Allow to collaborators Relationship`. 

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-07.png" alt=""/></a>
</p>

Here's an example of a todo object with related users. 

```javascript
{
   todo_id: '12345',
   title: 'Take out the trash',
   sm_owner: 'user/johndoe', //auto filled by StackMob
   collaborators: ['julie,bob,mike'], //a one-to-many relationship to schema "user"
   createddate: ...,
   lastmoddate: ...
}
```

*Your users must be logged-in and related to the individual object in order to take advantage of this access level.*

### Related Users on the sm_owner instance

`sm_owner` represents the user who created the object. Only allow access to users specified in a relationship field on the `sm_owner` user object.

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-08.png" alt=""/></a>
</p>

For instance, to share a photo with the photo sm_owner's friends, the `user` schema would have a `friends` relationship of type `user`, and the ACL permission for the `photo` schema would be set as seen below:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-09.png" alt=""/></a>
</p>

### Users defined in a Role

You can group your users by `Role`.  A role is basically a list of users grouped together.


To use roles, you'll first need to "Setup Role Permissions" while editing a <a href="https://dashboard.stackmob.com/schemas/" target="_blank">Schema</a> page. 

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-10.png" alt=""/></a>
</p>

You'll be prompted to create a new schema called  `aclrole`:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-11.png" alt=""/></a>
</p>

Once setup, you'll be able to add roles:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-12.png" alt=""/></a>
</p>

You can name roles and add users to the role.  *The users are your app users, not your StackMob user account.*  For example, the username for three user's of your app assigned to the admin role `sidney`, `erick`, and `ty`

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-14.png" alt=""/></a>
</p>

You can now assign permissions to this role for your schemas.

Assume you've created a `role` named `Admins`.  You can limit `read` access to Admins by setting them to `Allow to Admins`:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-13.png" alt=""/></a>
</p>

*You will need to setup your roles in Production as well since your users differ in Development and Production.*




### Allow to Any Logged In User

All objects in the schema are accessible to any logged-in user via the chosen REST actions (CREATE, READ, UPDATE, DELETE)

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-15.png" alt=""/></a>
</p>


*Your users must be logged-in in order to take advantage of this access level.*



<p>&nbsp;</p>

**Note about data created prior to June 14, 2012**

Because `sm_owner` is a new auto-generated field, data that already exists will not be able to take advantage of this feature.  StackMob will automatically auto generate/populate this field for all new data.  No developer action is required. 



**If the API request is signed with the private key, it will override these settings and will be allowed access.**



## Open (Accessible with public key OR private key)

At this permission level, anybody can perform the action on this object as long as they have the public key.  In OAuth 2.0 terms, because the public key is, well, public, that pretty much means anybody can make the call.  

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-16.png" alt=""/></a>
</p>


If the API request is signed with the private key, it will override this setting and will be allowed access.

This access level is available to the following SDKs:

* Javascript v0.4.0  and higher
* iOS v0.5.0 and higher
* Android 0.5.0  and higher


# How Schema Permissions affect development

With the introduction of Access Level Permissions, default schema permission settings have changed. 

You may find Schema Permissions at the bottom of your Create/Edit Schema page.

E.g., Bottom of the User schema:

<p class="screenshot">
<a href="https://dashboard.stackmob.com/schemas/" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/dashboard/acl-17.png" alt=""/></a>
</p>


## Automatically Generated Schemas: Open Permissions

Automatically generated schemas have different default schemas permissions depending on what SDKs you're using. Read on.

### iOS v0.4.12, Android v0.4.7 and JS SDK v0.3.0 and earlier SDKs (OAuth 1.0)

The permission levels for Create, Read, Update, and Delete will all be set to `Allow with private key` for *new* schemas created via <a href="http://www.stackmob.com/devcenter/docs/Automatic-Schema-Generation" target="_blank">automatic schema generation</a> if they used these SDKS to make the API call.

### iOS v0.5.0, Android v0.5.0 and  JS SDK v0.4.0 and above (OAuth 2.0)

The permission levels for Create, Read, Update, and Delete will all be set to `Open (Accessible with public key OR private key)` for *new* schemas created via <a href="http://www.stackmob.com/devcenter/docs/Automatic-Schema-Generation" target="_blank">automatic schema generation</a> if they used these SDKs to make the API call.  With permissions set to `Open`, this means that anybody with your public key can make these calls. 

### The User schema

StackMob provides the User schema for you automatatically when you create an app.  The <a href="http://www.stackmob.com/devcenter/docs/User-Schemas" target="_blank">User schema</a> will be set to `Open (Accessible with public key OR private key)` with permissions set to `Open`.  This means that anybody with your public key can make these calls. 


## Why default to Open?

With the introduction of OAuth 2.0, where SDKs make calls to the REST API with only the public key, `Open` schema permissions are set so that if a <a href="http://www.stackmob.com/devcenter/docs/Automatic-Schema-Generation" target="_blank">schema is generated automatically via an API call</a>, subsequent API calls in development are guaranteed to work in the interest of fluid development.

For example, **if** we had defaulted to `Private Key` permission, then SDKs using OAuth 2.0 (and passing up only the public key) would automatically generate a schema, then subsequently get blocked out because they aren't passing in public keys.  This obviously isn't too ideal!

## Action Needed During Development

As a result, **you will want to <a href="https://dashboard.stackmob.com/schemas/" target="_blank">update your schema permissions through the web platform</a> during development**.


It is **highly** recommended that you update the permissions to their desired (and likely more restricted) levels.

For your safety, we will require that you acknowledge your `Open` permissioned schemas in the <a href="http://www.stackmob.com/devcenter/docs/Deploying-your-App" target="_blank">deployment process</a>.

* Meanwhile, <a href="https://dashboard.stackmob.com/schemas/new" target="_blank">schemas created manually via the web platform</a> will retain permissions set by the developer.

### ACL not working in the Local Runner (JS SDK)?

If you are developing locally on the Local Runner and getting `401 Unauthorized` or similar rejection messages when querying for objects, try the following:

* If you are using `http://localhost:4567`, change to `http://127.0.0.1:4567`.
* <a href="https://dashboard.stackmob.com/schemas/" target="_blank">Check your schema permissions</a> to see if they are overly restrictively (Private Key, Not Allowed, etc)

# More Info: OAuth 2.0

If you'd like to read more about OAuth 2.0, you can read the <a href="http://www.stackmob.com/devcenter/docs/Authenticating-Users-with-the-JS-SDK" target="_blank">OAuth 2.0 docs for the JS SDK</a>. 


