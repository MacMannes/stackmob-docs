# REST Docs

StackMob's API is REST based.  Our SDKs abstract away the REST interface and provide a set of handy methods you can interact with when deleting, creating, updating and deleting objects.  We know sometime's you want to know what happening under the covers, so we've provided this REST reference.  It's also helpful for those building a custom implementation with StackMob that does not involve our SDKs.  We've also provided information on how we use OAuth 2 to create authorization headers for use when accessing schemas with permission levels enabled.

<p class="alert">StackMob supports both <a href="https://developer.stackmob.com/rest-api/oauth1-and-oauth2-guide" target="_blank">OAuth 1.0 and OAuth 2.0 protocols</a>.  </p>

Our Mobile SDKs use OAuth 2.0.  Our implementation of the OAuth 2.0 protocol is documented below and only uses the *public key* we assign your app.

Though our OAuth 1.0 implementation is not documented here, the REST API format is the same as the OAuth 2.0 format except that in OAuth 1.0, you *do not* need the `X-StackMob-API-Key` header.  Meanwhile, all OAuth 1.0 calls have an `Authorization` header that contains the OAuth 1.0 signed string as described in the example Twitter link above.  We use the <a href="http://tools.ietf.org/html/rfc5849" target="_blank" rel="nofollow">standard OAuth 1.0 spec</a>.  OAuth 1.0 uses your application's *public and private keys* that we assign you.  OAuth 1.0 signing is described in this <a href="https://dev.twitter.com/docs/auth/authorizing-request" target="_blank" rel="nofollow">Twitter OAuth 1.0 doc</a>.

<p class="alert alert-info">
  <a href="https://developer.stackmob.com/rest-api/oauth1-and-oauth2-guide" target="_blank">Read about how StackMob uses OAuth 1.0 and OAuth 2.0 protocols</a> and which one you should use.<br/><br/>People generally use OAuth 2.0 to take advantage of user authentication, access controls, and to not expose the private key on client side code.  (OAuth 2.0 only exposes the public key to the client.)<br/><br/>People generally use OAuth 1.0 on server side code to have access to all data.  OAuth 1.0 uses both the private and public key, and since the private key should only be known to the developer, privileged access to data is given via OAuth 1.0.  Since the calls originate from the server, private keys in the code are not exposed to the client.
</p>

# REST Full API Reference

# Create, Read, Update, Delete

## POST - Create Object

Create a new object in the datastore.

Let's create a `chatmessage` with `message=hello world!` and `author=johndoe`.

Request URL:

```bash
POST http://api.stackmob.com/chatmessage
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Content-Type: application/json
```

Request Body:

```javascript
{
  "message": "hello world!",
  "author": "johndoe"
}
```

Response shown in JSON notation for illustration.

```javascript
/*
 * Leave the primary key (chatmessage_id) out of the request body
 * and StackMob will generate a unique ID for you.
 * StackMob also generates the createddate and lastmoddate for you.
 */
{
  "chatmessage_id": "4e91523a9384bd92b93ecc80",
  "message": "hello world!",
  "author": "johndoe"
  "createddate": ...,
  "lastmoddate": ...
}
```

## Create Multiple Objects

You can also post multiple objects in a single API call. Let's create a few chat messages

Request URL:

```bash
POST http://api.stackmob.com/chatmessage
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Content-Type: application/json
```

Request Body:

```javascript
[
	{
		"message": "hello world!",
  		"author": "johndoe"
	},
	{
  		"message": "goodbye world!",
  		"author": "johndoe"
	}
]
```

## GET - Read Objects

Read your objects from the datastore.

## Get All Objects

You can fetch all instances of an object by querying with no parameters.

Here, we get all `user` objects.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "johndoe",
    "age": 25,
    "email": "...",
    "createddate": ...,
    "lastmoddate": ...
  },
  {
    "username": "jane",
    ...
  },
  {
    "username": "mary",
    ...
  },
  ...//all other users

]
```

## Find by ID

You can fetch a single object by querying (performing a GET request) by the primary key of a schema.

Here, get the `user` with `username=johndoe`.  `username` is the primary key.

Request URL:

```bash
GET http://api.stackmob.com/user/johndoe
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
{
  "username": "johndoe",
  "age": 25,
  "email": "...",
  "createddate": ...,
  "lastmoddate": ...
}
```

## PUT - Update Object

After you create an object, you may want to update only the values of select fields in that object.

Let's update `johndoe` so that he's `age=26` instead of `age=25`.

Request URL:

```bash
PUT http://api.stackmob.com/user/johndoe
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Content-Type: application/json
```

Request Body:

{
  "age": 26
}

Response shown in JSON notation for illustration.

```javascript
{
  "username": "johndoe",
  "age": 26, //updated with new value
  "email": "...",
  ...
}
```


## DELETE - Delete Object

You can delete a single object by primary key.

Let's delete a `user` with `username=johndoe`.

Request URL:

```bash
DELETE http://api.stackmob.com/user?username=johndoe
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response Headers:

```bash
HTTP/1.1 200 OK
```

# Special Headers

## X-HTTP-Method-Override

Occasionally you'll come across SDKs that have restrictions on the requests you send, such as not being able to access full RESTful capabilities. In the case of Unity, the only requests you can make are `GET` and `POST`. Thankfully, there are ways to circumvent these restrictions in the form of special headers.

X-HTTP-Method-Override:

```bash
X-HTTP-Method-Override: /* Your desired verb */
```

Please note that only `GET` and `POST` verbs can be overridden. These overrides fall into two categories, the 'payload' category and the 'no payload' category. What this means is that a no-payload request like `GET` can be overridden with `DELETE` and `HEAD`, whereas `POST` can only be overridden by `PUT`.

GET Override:

```bash
GET http://api.stackmob.com/user/1 HTTP/1.1
Content-Type: application/json
Accept: application/vnd.stackmob+json; version=0
X-HTTP-Method-Override: /* DELETE or HEAD */
```

POST Override:

```bash
POST http://api.stackmob.com/user HTTP/1.1
Content-Type: application/json
Accept: application/vnd.stackmob+json; version=0
X-HTTP-Method-Override: PUT
```

# Special Operations

## Binary File Upload

Uploading binary data is just like a regular Create (POST) or Update (PUT) call.  You simply need to set the value of the field in a specific format below, along with including the file as a base64 encoded string.

Request URL:

```bash
POST http://api.stackmob.com/user
or
PUT http://api.stackmob.com/user/john
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Content-Type: application/json
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```javascript
//POST
{
  "username": "john",
  "photo": "Content-Type: REPLACE_WITH_CONTENT_TYPE\nContent-Disposition: attachment; filename=REPLACE_WITH_FILE_NAME_NO_PATH_NEEDED\nContent-Transfer-Encoding: base64\n\nBASE_64_ENCODED_STRING_OF_THE_BYTE_ARRAY"
}

//PUT
{
  "photo": "Content-Type: REPLACE_WITH_CONTENT_TYPE\nContent-Disposition: attachment; filename=REPLACE_WITH_FILE_NAME_NO_PATH_NEEDED\nContent-Transfer-Encoding: base64\n\nBASE_64_ENCODED_STRING_OF_THE_BYTE_ARRAY"
}

//A "real" example of a PUT for a png file:
{
  "photo": "Content-Type: image/png\nContent-Disposition: attachment; filename=profile_pic.png\nContent-Transfer-Encoding: base64\n\niVBORw0KGgoAAAANSUhEUgAAA....."
}
```

Response shown in JSON notation for illustration.

```javascript
//StackMob will save the file to your S3 account and return the web path in its place:
{
  "username": "john",
  "photo": "http://..S3 Location../users/john/id/uploaded_file_name.png"
}
```

## Atomic Increment/Decrement

With atomic increment/decrement support you can update a field's integer value without having to worry about concurrency issues.

Suppose you had a schema `blog` with a `likes` field, and every time any user presses a button you want to increase this field by 1.

Increment the value as part of a regular "save" call:

<span class="method_signature">Increment - {fieldName[inc] : positive value}</span>
<span class="method_signature">Decrement - {fieldName[inc] : negative value}</span>

Request URL:

```bash
PUT http://api.stackmob.com/blog/1234
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Content-Type: application/json
```

Request Body:

```bash
{
  "likes[inc]": 1
}
```

Complete Sample cURL Request:

```bash
curl -sx localhost:8001 -H "Accept: application/vnd.stackmob+json; version=0" -H "Content-Type: application/json" -X PUT -d  "{\"likes[inc]\": 1}"  http://api.stackmob.com/blog/1234
```


## Updating a field value to NULL

To update a field value on an existing object to NULL, simply pass in the value "NULL" as a string.

Suppose you had a object with a `title` field with a value of "Big Kahuna", but you want the `title` field value to equal NULL.  Make an update request and set the `title` equal to the string "NULL".

Note: When creating a new object in the datastore, any existing fields not passed to the create API request will have a default value of NULL.

Request URL:

```bash
PUT http://api.stackmob.com/user/johndoe
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Content-Type: application/json
```

Request Body:

{
  "title": null
}


# StackMobQuery - Comparison, Select, Order, Pagination

In addition to the above basic CRUD methods, StackMob provides you ways to get results through filters/comparisons and also lets you paginate and sort the results.

## Equality Query

You can get an array of objects that match an equality statement.

Get all `users` whose `age=25`.

Request URL:

```bash
GET http://api.stackmob.com/user?age=25
GET http://api.stackmob.com/user?email[null]=true
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "johndoe",
    "age": 25,
    "email": "...",
    "createddate": ...,
    "lastmoddate": ...
  },
  {
    "username": "maryjane",
    "age": 25,
    ...
  },
  {
    "username": "johnsmith",
    "age": 25
    ...
  }
]
```

## Inequality Queries (!=, <=, >=, <, >, null)

StackMob gives you ways to filter your results with inequality queries.

The following shows how you can query a Twitter-like user of a certain `age` and a certain number of followers via `follower_count`.

Request URL:

```bash
//[ne] not equal, [null] is null
//[lt] less than, [lte] less than or equal to
//[gt] greater than, [gte] greater than or equal to
GET http://api.stackmob.com/user?age[ne]=10&follower_count[null]=false
GET http://api.stackmob.com/user?age[lt]=10&follower_count[gt]=20
GET http://api.stackmob.com/user?age[lte]=10&follower_count[gt]=20
GET http://api.stackmob.com/user?age[gt]=10&follower_count[gt]=20
GET http://api.stackmob.com/user?age[gte]=10&follower_count[gt]=20
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "johndoe",
    "age": ...,
    "follower_count": ..., //integer
    ...
  },
  {
    "username": "maryjane",
    "age": ...,
    "follower_count": ..., //integer
    ...
  },
  {
    "username": "johnsmith",
    "age": ...,
    "follower_count": ..., //integer
    ...
  }
]
```

## Querying for empty values

Because query parameters are interpreted literally, you cannot query for empty string values via `field=""`, 
instead you can use the [empty] operator in a similar manner as the [null] operator. Note that this operator only 
works for string fields.

Request URL:

```bash
GET http://api.stackmob.com/user?nickname[empty]=true
GET http://api.stackmob.com/user?nickname[empty]=false
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "johndoe",
    "nickname": "",
    ...
  }
]
```

## Querying for Multiple Values

Looking for particular values in a field?  Do so in one call by using "in" queries.

The following shows how you can look for more than one `user` in one call.  Look for "john123" and "johnsmith".

Request URL:

```bash
GET http://api.stackmob.com/user?username[in]=john123,johnsmith
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "john123",
    "age": ...,
    ...
  },
  {
    "username": "johnsmith",
    "age": ...,
    ...
  }
]
```

Looking for particular values not in a field?  Do so in one call by using "nin" queries.

The following shows how you can look for values not in a provided list. Look for values not equal to "john123" and "johnsmith".

Request URL:

```bash
GET http://api.stackmob.com/user?username[nin]=john123,johnsmith
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "notjohn123",
    "age": ...,
    ...
  },
  {
    "username": "notjohnsmith",
    "age": ...,
    ...
  }
]
```

## Querying Arrays

The following shows how you can search for values within an array.  It also uses the `in` query as noted above.

Let's get all `user`s who're friends with "johndoe".

Request URL:

```bash
GET http://api.stackmob.com/user?friends[in]=johndoe
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Response shown in JSON notation for illustration.

```javascript
[
  {
    "username": "john123",
    "age": ...,
    "friends": ["maryjane", "johndoe", "johnsmith"],
    ...
  },
  {
    "username": "johnsmith",
    "age": ...,
    "friends": ["johndoe", "maryjane"],
    ...
  }
]
```

## OR and AND Queries

StackMob supports logical conjunctions in certain configurations. In a simple query with multiple statements, they're joined by AND, as you'd expect:

```bash
GET http://api.stackmob.com/user?age=25&location=NYC
```

StackMob can also accept boolean logic of the form:

```
(A && (B || C || ...)
```

where each letter represents any number of clauses ANDed together. To represent this nested in a query, each equality statement is prefixed with its location within the boolean logic. Thus this expression:

```
(age == 25 || location == NYC)
```

would be represented by

```bash
GET http://api.stackmob.com/user?[or1].age=25&[or1].location=NYC
```

To take a more complex example, this expression

```
(age > 25 && ((location == NYC && name != john) || (location == SF && name != Mary) || location == LA))
```

would be represented by

```bash
GET http://api.stackmob.com/user?age[gt]=25&[or1].[and1].location=NYC&[or1].[and1].name[ne]=john&[or1].[and2].location=SF&[or1].[and2].name[ne]=Mary&[or1].location=LA
```
You can only have the one block of or clauses, so (A || B) && (C || D) isn’t allowed. Each or clause can have an arbitrary number of and statements, but you can’t nest any deeper than that. This allows us to use indexes efficiently so that your queries run blazingly fast. This level of complexity should be sufficient for most real world applications. In theory any boolean expression can be converted to a compatible form, and so long as your expression isn’t too complicated it may well be practical too. Due to underlying restrictions we also disallow or queries with sorting or geo conditions like near.

## Selecting Fields to Return

By default, StackMob returns every field that is defined in your schema. You can use the `X-StackMob-Select` header to limit the number of fields in the returned data object. In this example only the `username` and `age` fields on the object are returned.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
Accept: applicaton/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Select: username,age
```

Response shown in JSON notation for illustration.

```javascript
//Note that fields other than username and age are excluded
[
  {
    "username": "jane",
    "age": 21
  },
  {
    "username": "jill",
    "age": 34
  },
  {
    "username": "andrew",
    "age": 18
  }
]
```

This feature works well with the expand functionality. Use the expand setting to set the depth of the object expansion, and then use the select feature to pick which fields are returned. You will get an error, if you try to select a field in an object, which you did not expand.

To access subobjects use the format `referenceField.referredField`, where `referenceField` is the field in the object that contains the reference, and `referredField` is the name of the field that you want to select. Suppose you have a schema with a `user` object, which has a field called `interest_ref`, which refers to an object called `interest`, and you want to select only username and the field `interest_type` in the interest object.

Request URL:

```bash
GET http://api.stackmob.com/user
```


Request Headers:

```bash
Accept: applicaton/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Expand: 1
X-StackMob-Select: username,interest_ref,interest_ref.interest_type
```

Response shown in JSON notation for illustration.

```javascript
//Note that other fields are excluded
[
  {
    "username": "jane",
    "interest_ref":
		{
		  "interest_type": "sports"
		}
  },
  {
    "username": "jill",
    "interest_ref":
		{
		  "interest_type": "academic"
		}
  },
  {
    "username": "andrew",
    "interest_ref":
		{
		  "interest_type": "academic"
		}
  }
]
```

You can chain the reference fields as far as you have expanded the objects. For example, you if your `user` object has a `friend` field, which refers to another `user` object, you can select the user's friend, and the friend's interest.

Use the wildcard `*` character to select all the fields in an objects.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Expand: 2
X-StackMob-Select: username,friend,friend.username,friend.interest_ref,friend.interest_ref.*
```

Response shown in JSON notation for illustration.

```javascript
//Note that other fields are excluded
[
  {
    "username": "jane",
    "friend":
      {
        "username": "bob",
      },
    "interest_ref":
	  {
	    "interest_type": "sports",
	    "name": "soccer"
	  }
  },
  {
    "username": "jill",
    "friend":
      {
        "username": "andrew",
      },
    "interest_ref":
	  {
	    "interest_type": "academic",
	    "name": "science"
	  }
  },
  {
    "username": "andrew",
    "friend":
      {
        "username": "jill",
      },
    "interest_ref":
	  {
	    "interest_type": "academic",
	    "name": "math"
	  }
  }
]
```

Note, that you will always get back the primary key field of each object you request. Conversely, `password` fields will never be returned.



## Order By

Sort your results using the Order By feature.

Let's sort a `user` by `age` and then the date the object was created.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-OrderBy: age:asc,createddate:desc
```

Response shown in JSON notation for illustration.


```javascript
//Order by age ascending, then by date descending
[
  {
    "username": "jeff",
    "age": 6,
    "createddate": *Nov 11, 2011* //utcmillisconds converted to date here for example
  },
  {
    "username": "john",
    "age": 10,
    "createddate": *Nov 11, 2011* //utcmillisconds converted to date here for example
  },
  {
    "username": "jane",
    "age": 15,
    "createddate": *Nov 9, 2011* //utcmillisconds converted to date here for example
  },
  {
    "username": "mary",
    "age": 15,
    "createddate": *Nov 6, 2011* //utcmillisconds converted to date here for example
  }
]
```

## Pagination

Do you have hundreds of users and only want to get 10 at a time?  You can paginate through any of your GET queries above using pagination. All range queries are *zero-indexed* and *inclusive*.

Let's get the first 10 `user` objects.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
Range: objects=0-9
```

Response shown in JSON notation for illustration.

```javascript
[
  { //result 0
    "username": "jeff",
  },
  ... //results 1, 2, 3, 4, 5, 6, 8
  {
    //result 9
    "username": "janet",
    ...
  }
]
```

# Arrays and Relationships

You can use the StackMob Array and Relations API to easily manage your arrays and relationships in one call.

* Create and save instances of related objects, then subsequently save them to the parent object, in one call.
* Get related objects along with the parent object in one call.
* Append values to an array on your object without updating the entire object.
* Delete values from an array without updating the entire object.

## POST - Creating and Appending Related Objects

You can create and save related objects, then subsequently save them to their parent object, in one call.

*This is instead of making a single call for each object, then another call to update the parent object with the relationships.*

```bash
POST /PRIMARY_SCHEMA/PRIMARY_ID/RELATED_FIELD_NAME
```

Request Body: JSON Array of JSON Documents

Example:

```bash
POST /accounts/some_user/items HTTP/1.1
…
Content-Type: application/json;charset=UTF8
…

[
  {"item_id": "1",
   "text": "my first todo"},
  {"item_id": "2",
   "text": "my other today"}
]
```

## Create or Update Multiple Related Objects

Relationships can use a specialized method of posting data that combines POST and PUT, while also submitting expanded json trees all at once. StackMob can easily accept a whole json tree containing multiple related objects, but json lacks any sort of type hints to distinguish which objects go with which schemas, so that information is supplied in a header. Without the header, the expanded json tree will be rejected for having subobjects. It's also the case that when posting objects together like this, some may be new and you want them created, while some exist already, and need to be updated. Thus we also "upsert" the objects, creating them if necessesary, otherwise updating them.

Let's say our `chatmessage` schema has a one to one relation to the `user` schema called `author`.

Request URL:

```bash
POST http://api.stackmob.com/chatmessage
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Relations: author=user
Content-Type: application/json
```

Request Body:

```javascript
[
  {
    "message": "hello world!",
      "author": {
      "username": "johndoe",
      "age": 27,
      "hobby": "fishing"
    }
  }
]
```

The `X-StackMob-Relations` header tells StackMob the the author key corresponds to the user schema. It's structured like a query string, and each field containing a json subobject must be included. Here's a more complex example to show how it works more than one level deep. Say that the `user` schema has a one to many relation to the `user` schema called `friends`.


Request URL:

```bash
POST http://api.stackmob.com/chatmessage
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Relations: author=user&author.friends=user
Content-Type: application/json
```

Request Body:

```javascript
[
  {
    "message": "hello world!",
      "author": {
      "username": "johndoe",
      "age": 27,
      "hobby": "fishing"
      "friends": [{ "username": "janedoe", "age": 34 }, { "username": "bob", "age": 25 }]
    }
  }
]
```

Nesting is shown with the dot syntax, i.e. `author.friends=user`.  There is a limit of 3 nested objects. The same syntax applies whether you are referring to a one-to-one or one-to-many relation.

## GET - Expanding Relationships: Get Full Objects, not just IDs

Relationships are represented in your object schemas as arrays of primary IDs. You can return an array of objects instead.

Assume you have `user` schema with a `friends` relationship field. Normally the `friends` relationship field is represented as an array of primary keys (usernames), but you can use the `expand` feature to have that array expanded as an array of full `user` objects instead.

To do this, add the `X-StackMob-Expand` header with a value of 1 - 3, depending on how many levels of related objects you want to return.

Request URL:

```bash
GET http://api.stackmob.com/user
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-Expand: /* 1 - 3 */
```

Response shown in JSON notation for illustration.

```javascript
// Get without "expand"
{
  "username": "john",
  "friends": ["jane","jill","andrew"]
}

// Get WITH "expand" = 1
{
  "username": "john",
  ...
  "friends": [
    {
      "username": "jane",
      "age": ...,
      ...
    },
    {
      "username": "jill",
      "age": ...,
      ...
    },
    {
      "username": "andrew",
      "age": ...,
      ...
    }
  ]
}
```

## PUT - Appending values to an array or Add an existing object to a relationship

Use this to append values to an array without updating the whole object.  It also removes concurrency issues if two users are modifying the same object at the same time since both people are just appending.

* If the field is an array, the values will be appended to it with no uniqueness constraint.
* If the field is a relationship, just append the object IDs to the array, not the objects themselves.  The resulting array will be deduped so that there are no duplicate references to related object IDs.

```bash
PUT /PRIMARY_SCHEMA/PRIMARY_ID/RELATED_FIELD_NAME
```

Request Body: JSON Array of Primitive JSON Values

Example:

```bash
PUT /accounts/some_user/items HTTP/1.1
…
Content-Type: application/json;charset=UTF8
…

[1, 2, 3, 4]
```

## DELETE - Deleting Related Objects, Breaking Relations, and Pulling Values from Arrays

* Delete values from an array without updating the entire object
* Delete related object reference from parent object.  The related objects will still exist in the StackMob datastores.
* Delete related objects from the StackMob datastores.  Also delete the reference from the parent object.

```bash
DELETE /PRIMARY_SCHEMA/PRIMARY_ID/RELATED_FIELD_NAME/ID1[,ID2,ID3,...]
```

Request Headers:

```bash
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-CascadeDelete: true
```

if set to any value other than false cascading deletion will be applied to the request (if it is omitted it is the same as specifying "false" as the value). 

Request Body: *Empty*

Example:

```bash
DELETE /accounts/some_user/items/1,2 HTTP/1.1
…
X-StackMob-CascadeDelete: true
…
```

# Geospatial Queries

If you have a `geopoint` field defined in your model, there are two types of geospatial queries available: `near` and `within`. All `geopoints` are longitude, latitude points on the Earth in decimal degrees with `double` precision. Longitudes must be in the range `-180` inclusive to `180` exclusive, and latitudes must be in the range `-90` inclusive to `90` inclusive. All GeoQueries take the curvature of the Earth into account when calculating distances.

## Near Queries

`near` queries search for results at a specified longitude,latitude coordinate in degrees. A third parameter, the maximum distance in radians, is optional. `near` queries will always return results sorted by distance from the queried point (`X-StackMob-OrderBy` headers will be ignored), and the results will embed a "distance" field into the geopoint field of the queried object. The StackMob sdk's provide convenience methods for converting between miles and kilometers to radians automatically. The following example will find all objects whose `location` field is within a 10 mile radius of San Francisco, CA sorted by distance.

Parameters for all geo queries are provided as a comma separated list. The first parameter is *latitude*, the second parameter is *longitude*, and the third (optional) parameter is *distance in radians*. Radial distances are calculated by dividing the distance by the Earth's radius (3956.6mi or 6367.5km). For example, 10 miles in radians would be 10mi / 3956.6mi = 0.0025274225345 radians.  And 10 kilometers in radian would be 10km / 6367.5km = 0.00157047506871 radians.

Request URL:

```bash        
  GET http://api.stackmob.com/myobject?mygeofield[near]=37.73,122.68,.00252 //lat,lon,distance (radians)
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

```javascript
[
  {
    ...,
    "mygeofield" : { "lon" : x, "lat" : y, "distance": z } 
  },
  ...
]
```

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<error message>"}
```

## Within Queries

`within` queries search for results within a specified radius of a given point or within a 2-dimensional box. These queries do not have a natural ordering. Unlike `near` queries, if you specify an ordering for the result set it will be honored.


Request URL:

To query `within` a radius, specify a lon/lat point in decimal degrees with a radius in radians:

```bash
GET http://api.stackmob.com/myobject?mygeofield[within]=37.75,122.68,.00252  //lat,lon,distance (radians)
```

To query `within` a bounding box, specify two lon/lat points. The first is the lower left, and the second is the upper right:

```bash
GET http://api.stackmob.com/myobject?mygeofield[within]=37.75,122.68,37.95,122.78 //lat1, lon1, lat2, lon2
```
Request Headers:

```bash

  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash

*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

```javascript
[
  {
    ...,
    "mygeofield" : { "lon" : x, "lat" : y} 
  },
  ...
]
```

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<error message>"}
```

# User Authentication: OAuth 2.0

StackMob gives you the ability to login with a user object via a user-specified `username` and `password`.  This uses `user` schemas.  By default, StackMob provides a `user` schema for you, but you can always create your own.  (Just be sure to check off "Create as User Schema" in the Create Schema UI if so.)

StackMob uses OAuth 2.0 to authenticate your users.  The following covers StackMob's implementation of OAuth 2.0 and how to implement it via the REST API.

## Logging in

Let's login user "Bruce Wayne" with password "imbatman".  

We'll assume that you're using the default provided `user` schema, hence the `https://../user` URLs.

Your REST API call will have the format of:

(note that the Content Type is `application/x-www-form-urlencoded` unlike most of our other API calls)

```xml,5
Request URL:
POST https://api.stackmob.com/user/accessToken

Request Headers:
Content-Type:application/x-www-form-urlencoded
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */

Request Body:
username=Bruce%20Wayne&password=imbatman&token_type=mac
```

You'll be given back the following in the response:

```js,2,3,6
{
  "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
  "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
  "mac_algorithm":"hmac-sha-1",
  "token_type":"mac",
  "expires_in":3600,
  "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
  "stackmob":{
    "user":{"username":"Bruce Wayne", 
    ...
    } 
  }
}
```

* Your app should save the `access_token` and `mac_key` somewhere you can refer to for the remainder of the session.  
* Similarly, because the access token only has one hour to live (as given by `expires_in`), it's recommended you save the expire time somewhere so you can check for the validity of the token. This is not required.  It's simply so that your client can check to see if the OAuth tokens are still valid by checking the local time against the expire time.  

e.g. `expireTime = (new Date()).getTime() + 3600 * 1000);`

* Also for convenience, it's recommended you save the logged in user locally for reference as well:  `theReturnedJSON['stackmob']['user']['username']`.  This is for convenience so that you can display who's logged in.

You can store the above into HTML5 Local Storage, for instance.


## Signing Requests

To take advantage of ACL permissions, you'll want to sign your requests so that StackMob knows what user is making the request.  First, formulate your REST API request as you normally would (see REST API docs for StackMob functionality).  Then, if the user is logged in, you'll want to include an extra HTTP request header: `Authorization`.  Below, we'll go over how to generate the `Authorization` string.  Example of request to get all books.

```xml,8
Request URL:
https://api.stackmob.com/books

Request Headers:
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-User-Agent: { Your User Agent.  Recommended format: Your SDK Version # }
Authorization:MAC id="vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",ts="1343427512",nonce="n2468",mac="79js6rr3ynOCyssOHuGpGikfpvs="
```

Read on to see how to generate the `Authorization` header.

**Generating the Authorization Header**

StackMob follows the OAuth 2.0 spec for signing Authorization headers as depicted here:  [http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01](http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01)

StackMob uses *hmac-sha-1* and then base64 encodes the result along with some other items.

Don't forget that `http` is port 80 and `https` is port 443!  The port number is used in the signing.


**StackMob OAuth 2.0 Implementation Examples (iOS/JS)**

Here's an example of StackMob's JS SDK implementation of the signing: [https://gist.github.com/9515a7ecdbb5625b348b](https://gist.github.com/9515a7ecdbb5625b348b)

Here's an example of StackMob's iOS SDK implementation of the signing: [https://gist.github.com/3410085](https://gist.github.com/3410085)

## Keeping a User Logged In

Access tokens expire after the seconds specified in the return JSON's `expires_in` field.  At this point the user is logged out on the server side.  To keep them logged in, you can make a `refreshToken` API call using the `refresh_token` returned in the original login request.

```js,7,8
    /* JSON response from /user/accessToken call */
    {
      "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
      "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
      "mac_algorithm":"hmac-sha-1",
      "token_type":"mac",
      "expires_in":3600,
      "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
      "stackmob":{
        "user":{"username":"Bruce Wayne", 
        	...
       	} 
      }
    }
```

Make a call to get a new access token by calling the `refreshToken` method.  Doing so keeps the user logged in without having to prompt them again for a username/password.  Instead, use the `refresh_token`.

```xml,2,11
Request URL:
POST https://api.stackmob.com/user/refreshToken

Request Headers:
Content-Type:application/x-www-form-urlencoded
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */

Request Body:
refresh_token=nZSiH3L5K4febMlELguILucrWpjRud56&grant_type=refresh_token&token_type=mac&mac_algorithm=hmac-sha-1
```

You'll be given this JSON in response.  Notice you get a new refresh_token for next time.

```js,7
{
  "access_token":"ILY3seHWbRjoaPyhJ0LzBimJVNkaq0OQ",
  "mac_key":"ixCKDje4Tak8tx3yYqkFUBWKY3RypUBR",
  "mac_algorithm":"hmac-sha-1",
  "token_type":"mac",
  "expires_in":3600,
  "refresh_token":"gy2satZp733QDv7Wb65SH1JIMKw4O7XL",
  "stackmob":{
    "user":{ "username":"Bruce Wayne", 
    	...
    }
  }
}
```

Look familiar?  It's the same response as the one you got via the `accessToken` call.  Process it as you did before.

To keep the user logged in, repeat this process.  

As an example, in the official StackMob JS SDKs, we locally save the expiration time of the access token, and when you make an API call after that access token has expired, we put the original API call on hold, make the refreshToken call to log the user back in, then run the original API call behind the scenes.

## Logout

Logout "johndoe":

Request URL:

```bash
    GET https://api.stackmob.com/user/logout
```

Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

    HTTP/1.1 200 OK
  
    

Response (Failure):

    HTTP/1.1 401 Unauthorized
    
# Password management
StackMob gives you a few utility methods for dealing with common password usecases.

## Forgot Password
The Forgot Password call can be hooked up to a button on the login screen of your app to allow users to reset their password via email. To do this you need a user schema that has the `Reset Password` API Method enabled, and you need to specify the field that represents an email field to which StackMob will send a temporary password. This can be done on your Edit Schema page for your `user` schema.

*From the <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">Edit User Schema page</a>:*
<p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/StackMob_Forgot_Password_User_Schema.png" alt="Forgot Password"/></p>

Make a call to StackMob requesting a new password by email.  The email will be sent to the value of the field specified in your Forgot Password dropdown above.  If you selected `username` and this user's username was `example_john@stackmob.com`, then we'll send a temporary password to `example_john@stackmob.com`.

Request URL:

    POST http://api.stackmob.com/user/forgotPassword

Request Headers:

    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */

Request Body:

    {"username": USERNAME_OF_REQUESTED_USER }

<hr/>

Response (Success):

    HTTP/1.1 200 OK
    Set-Cookie: <cookie>
    Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

    HTTP/1.1 401 Unauthorized
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
{"error":"<message>"}
```

The user will receive an email with a temporary password valid for 24 hours. The temporary password must be reset by the user before the user can login again. To do this after invoking the `forgotPassword` call, your app should redirect to a reset password screen with fields for the old and new password. The user can get the temporary password from their email, come back to your app, and submit the form. Once the form is submitted you can call login as shown below, with the temporary and new passwords passed as `password` and `new_password` respectively.

## Login with Temporary Password (and reset with new password)

Request URL:
    
    GET http://api.stackmob.com/user/accessToken?password=temppassword&username=johndoe&new_password=newpassword

Request Headers:
    
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */

Request Body:

*Empty*
<hr/>

Response (Success):

    HTTP/1.1 200 OK
    Set-Cookie: <cookie>
    Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

    HTTP/1.1 401 Unauthorized
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
{"error":"<message>"}
```

Your user may also close the app and attempt to use the regular login screen with their temporary password. When this happens your call to login will fail with the following error:

```javascript
{ "error": "Temporary password reset required" }
```

Your app should parse the returned login errors for the string above.  This is StackMob's message to your app indicating that the temporary password was accepted but the user must now select a new password. 

When your app sees this error, you should display a reset password screen. Once they've done that and entered their new password, call login again with the temporary password and an extra field `new_password` to change their password as shown above.

## Reset Password
Reset password, totally distinct from the forgot password process above, is a general way to handle a user resetting their password. Given an old and new password it verifies the old one against the users's, and if successful switches to the new one. At the moment it's possible to simply do a post request and replace a password field, but we plan to restrict that on new schemas soon. You should use resetPassword going forward as a best practice. *This method requires that the user be logged in.*

Request URL:

    POST http://api.stackmob.com/user/resetPassword

Request Headers:

    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */

Request Body:

    {"old": {"password": "[tempPassword]"}, "new": {"password": "[newPassword]"}}

<hr/>

Response (Success):

    HTTP/1.1 200 OK
    Set-Cookie: <cookie>
    Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

    HTTP/1.1 401 Unauthorized
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
{"error":"<message>"}
```

# Facebook

## Create User with Facebook

Create a user and link the Facebook account access token. When they do this, it will actually create a StackMob user behind the scenes and link the provided access token.

Create user with Facebook:

Request URL:

```bash
   POST http://api.stackmob.com/user/createUserWithFacebook
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
    {"username": "<user>",	
     "fb_at": "<accesstoken>"}
```
<hr/>

Response (Success):

  HTTP/1.1 201 Created
  Set-Cookie: <cookie>
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Login with Facebook and create user if needed

Have your users login with their Facebook account.

**Note:** This endpoint will create a StackMob user and link them with the provided Facebook access token is one does not already exist.

After logging into Facebook using an appropriate Facebook SDK, you need to let StackMob know of the login.  To do so, use the resulting Facebook Access Token to login to your app.

Pass in the Facebook Access Token `fb_at` along with `token_type=mac` in your request.  Optionally, pass in a `username` key and value that will be assigned if a user doesn't already exist.

Request URL:

```bash
POST https://api.stackmob.com/user/facebookAccessTokenWithCreate
```

Request Headers:

```bash
Content-Type:application/x-www-form-urlencoded
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */
```

Request Body:

```bash
{ token_type=mac&fb_at=<fbtoken>&username=<username> }
```

You'll be given back the following in the response:

```js
{
  "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
  "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
  "mac_algorithm":"hmac-sha-1",
  "token_type":"mac",
  "expires_in":3600,
  "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
  "stackmob":{
    "user":{"username":"Bruce Wayne", 
    ...
    } 
  }
}
```

Process the response as you would with a regular login call.  (See <a href="#a-logging_in">User Authentication: OAuth 2.0, Logging In</a>)

## Login with Facebook

Have your users login with their Facebook account.

**Note:** This only works with a StackMob user that has already been associated with a Facebook account via `create user with Facebook` or `link user with Facebook`

After logging into Facebook using an appropriate Facebook SDK, you need to let StackMob know of the login.  To do so, use the resulting Facebook Access Token to login to your app.  Note that you can only login a known StackMob user, so be sure to use "createUserWithFacebook" or "linkUserWithFacebook" first.  Otherwise your login attempt may result in a 500 error.

Pass in the Facebook Access Token `fb_at` along with `token_type=mac` in your request.

Request URL:

```bash
POST https://api.stackmob.com/user/facebookAccessToken
```

Request Headers:

```bash
Content-Type:application/x-www-form-urlencoded
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */
```

Request Body:

```bash
{ token_type=mac&fb_at=<fbtoken>&username=<username> }
```

You'll be given back the following in the response:

```js
{
  "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
  "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
  "mac_algorithm":"hmac-sha-1",
  "token_type":"mac",
  "expires_in":3600,
  "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
  "stackmob":{
    "user":{"username":"Bruce Wayne", 
    ...
    } 
  }
}
```

Process the response as you would with a regular login call.  (See <a href="#a-logging_in">User Authentication: OAuth 2.0, Logging In</a>)

## Linking Facebook to an existing User object

If they already have a StackMob user account and they'd like to link their Facebook account with it, then they can login via their StackMob user account and then you can link the two via:

Request URL:

```bash
  GET http://api.stackmob.com/user/linkUserWithFacebook?fb_at=<accesstoken>
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Get Facebook User Info

Get Facebook Info for the Currently Logged in User:

Request URL:

```bash
    GET http://api.stackmob.com/user/getFacebookUserInfo
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
{<Facebook user info>}
```

## Post Message to Facebook

Post message to Facebook:

Request URL:

```bash
    GET http://api.stackmob.com/user/postFacebookMessage?message=xyz
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X


## Unlinking Facebook from a linked User object

If a user would at some point like to unlink their StackMob user account from their Facebook account, you can allow this via:

Request URL:

```bash
  DELETE http://api.stackmob.com/user/unlinkUserFromFacebook
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (User not linked)

  HTTP/1.1 404 Not Found
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):
  
  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<error message>"}
```

# Twitter

## Create User with Twitter

Create a user and link the Twitter account access token. When they do this, it will actually create a StackMob user behind the scenes and link the provided access token.

Create user with Twitter:

Request URL:

```bash
  POST http://api.stackmob.com/user/createUserWithTwitter
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
    {"username": "<username>",
     "tw_tk":     "<token>",
     "tw_ts":     "<tokensecret>"} 
```

<hr/>

Response (Success):

  HTTP/1.1 201 Created
  Set-Cookie: <cookie>
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Login with Twitter and create user if needed

Have your users login with their Twitter account.

**Note:** This endpoint will create a StackMob user and link them with the provided Facebook access token is one does not already exist.

After logging into Twitter using an appropriate Twitter SDK, you need to let StackMob know of the login.  To do so, use the resulting Twitter Access Token to login to your app.

Login with Twitter:

Request URL:

```bash
  POST http://api.stackmob.com/user/twitterLoginWithCreate
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=0
  Content-Type:application/x-www-form-urlencoded
  X-StackMob-API-Key: /* Your Public Key */
  X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */
```

Request Body:

```bash
{ mac_algorithm=hmac-sha-1&token_type=mac&tw_tk=<token>&tw_ts=<tokensecret>&username=<username> }
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Set-Cookie: <cookie>
  Content-Type: application/vnd.stackmob+json; version=X

```javascript
{"username":"johndoe","tw":{<Twitter user info>}}
```

Response (Failure):

  HTTP/1.1 401 Unauthorized
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Login with Twitter

Have your users login with their Twitter account.

**Note:** This only works with a StackMob user that has already been associated with a Twitter account via `create user with Twitter` or `link user with Twitter`

After logging into Twitter using an appropriate Twitter SDK, you need to let StackMob know of the login.  To do so, use the resulting Twitter Access Token to login to your app.  Note that you can only login a known StackMob user, so be sure to use "createUserWithTwitter" or "linkUserWithTwitter" first.  Otherwise your login attempt may result in a 500 error.

Login with Twitter:

Request URL:

```bash
  POST http://api.stackmob.com/user/twitterLogin
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=0
  Content-Type:application/x-www-form-urlencoded
  X-StackMob-API-Key: /* Your Public Key */
  X-StackMob-User-Agent: /* Your User Agent.  Recommended format: Your SDK Version # */
```

Request Body:

```bash
{ mac_algorithm=hmac-sha-1&token_type=mac&tw_tk=<token>&tw_ts=<tokensecret> }
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Set-Cookie: <cookie>
  Content-Type: application/vnd.stackmob+json; version=X

```javascript
{"username":"johndoe","tw":{<Twitter user info>}}
```

Response (Failure):

  HTTP/1.1 401 Unauthorized
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Linking Twitter to an existing User object

If they already have a StackMob user account and they'd like to link their Twitter account with it, then they can login via their StackMob user account and then you can link the two via:

Request URL:

```bash
  GET http://api.stackmob.com/user/linkUserWithTwitter?tw_ts=<tokensecret>&tw_tk=<token>
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<error message>"}
```

## Get Twitter User Info

Get Twitter User Info:

Request URL:

```bash
    GET http://api.stackmob.com/user/getTwitterUserInfo
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
{<Twitter user info>}
```

## Update Twitter Status

Update Twitter status:

Request URL:

```bash
    GET http://api.stackmob.com/user/twitterStatusUpdate?tw_st=xyz
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```

<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X

## Unlinking Twitter from a linked User object

If a user would at some point like to unlink their StackMob user account from their Twitter account, you can allow this action for a currently logged-in user via:

Request URL:

```bash
  DELETE http://api.stackmob.com/user/unlinkUserFromTwitter
```
Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (User not linked)

  HTTP/1.1 404 Not Found
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):
  
  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<error message>"}
```

# Gigya

## Creating and logging in with Gigya

Gigya is a service that facilitates login with various social networks. The Gigya flow does not require you to explicitly create users on StackMob, a user will be created automatically if valid Gigya authentication credentials are provided.

When you successfully log in your user to Gigya, Gigya will return a user object. You will need to get the following fields from the gigya user object: "UID", "signatureTimestamp", "UIDSignature", and POST the credentials to gigyaAccessToken endpoint.

```bash
    POST http://api.stackmob.com/user/gigyaAccessToken
```

Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```bash
{ gigya_uid=UID&gigya_ts=signatureTimestamp&gigya_sig=UIDSignature&token_type=mac }
```

<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

```js
{
  "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
  "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
  "mac_algorithm":"hmac-sha-1",
  "token_type":"mac",
  "expires_in":3600,
  "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
  "stackmob":{
    "user":{"username":"Bruce Wayne", 
    ...
    } 
  }
}
```

Response (Failure):

  HTTP/1.1 401 Unauthorized
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Linking Gigya token to an existing User object

Allows for linking the current logged in StackMob user with the Gigya credential information provided. Upon success you will be able to login the user with the Gigya login method.

You will need to get the following fields from the gigya user object: "UID", "signatureTimestamp", "UIDSignature", and POST the credentials to gigyaAccessToken endpoint.

```bash
    POST http://api.stackmob.com/user/linkUserWithGigya
```

Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```bash
{ gigya_uid=UID&gigya_ts=signatureTimestamp&gigya_sig=UIDSignature&token_type=mac }
```

<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 401 Unauthorized
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

## Unlinking Gigya token from existing user object

Allows for unlinking the current logged in StackMob user with their linked Gigya credential information. Upon success you will no longer be able to login the user with the Gigya login method.

```bash
    DELETE http://api.stackmob.com/user/unlinkUserFromGigya
```

Request Headers:

```bash
  // "version" sets your REST API Version. "0" for Development. "1" and up for Production
  Accept: application/vnd.stackmob+json; version=X
  X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```bash
*Empty*
```

<hr/>

Response (Success):

  HTTP/1.1 200 OK
  Content-Type: application/vnd.stackmob+json; version=X

Response (User not linked):

  HTTP/1.1 404 Not Found
  Content-Type: application/vnd.stackmob+json; version=X

Response (Failure):

  HTTP/1.1 400 Bad Request
  Content-Type: application/vnd.stackmob+json; version=X
  
```javascript
{"error":"<message>"}
```

# Custom Code

## Making GET/DELETE Calls

Make a GET or DELETE call to custom code.  Assume the custom code method is `helloworld`:

Request URL:

```bash
    GET http://api.stackmob.com/helloworld?param1=value1&param2=param2 (parameters optional - dependent on your method)
    
    or 
    
    DELETE 
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```bash
*Empty*
```
<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
JSON: Your Custom Code Method Response
```

## Making PUT/POST Calls


Make a PUT or POST call to custom code.  Assume the custom code method is `helloworld`:

Request URL:

```bash
    PUT http://api.stackmob.com/helloworld
    
    or 
    
    POST 
```
Request Headers:

```bash
    // "version" sets your REST API Version. "0" for Development. "1" and up for Production
    Accept: application/vnd.stackmob+json; version=X
    X-StackMob-API-Key: /* Your Public Key */
```
Request Body:

```javascript
//Optional.  You can pass any string, e.g., JSON
{
  param1: value1,
  param2: value2
}
```
<hr/>

Response:

    HTTP/1.1 200 OK
    Content-Type: application/vnd.stackmob+json; version=X
    
```javascript
JSON: Your Custom Code Method Response
```


# Push Notifications

Engage your users with Push notifications.  StackMob provides a push service for your application through a REST API.

## Setting up Push Notifications

* [Setup iOS Push](http://www.stackmob.com/devcenter/docs/Getting-Started-with-iOS-Push)
* [Setup Android Push](http://www.stackmob.com/devcenter/docs/Setting-up-Android-Push)

This older, deprecated Android Push framework can still be used for legacy apps. The android push above is recommended for new apps.

* [Setup Android C2DM Push](http://www.stackmob.com/devcenter/docs/Setting-up-Android-C2DM-Push)

## Device Tokens

The core abstraction that the push API uses is called the device token. A device token looks like this in JSON (note that this example doesn't have a real token in it!):

```javascript
  {
    "type": "androidgcm",
    "token": "a88fqsdg8as87rgq87wrg8as8dg78zDf8a98sdf98asd8fa7sdf"
  }
```

Currently, we support "androidgcm" and "ios" in the type field, and the "token" field is the [registration ID](http://www.stackmob.com/devcenter/docs/Setting-up-Android-Push#a-1._creating_a_new_registration_id) on Android, and the [iOS device token](http://www.stackmob.com/devcenter/docs/Getting-Started-with-iOS-Push#a-getting_a_valid_device_token_for_your_mobile_device) on iOS. Note that iOS tokens must be stripped of all " " (space), "<" and ">" characters before being sent to StackMob.

## Register a Device to Receive Push Notifications

StackMob will help you manage the device tokens for users who have subscribed to receive push notifications. We also allow you to optionally link a StackMob username to a device token so that you can send a message to all devices that belong to a specific user, or even send a message to all users.

```bash
POST http://push.stackmob.com/tokens/android/a88fqsdg8as87rgq87wrg8as8dg78zDf8a98sdf98asd8fa7sdf
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```javascript
{
  "user": "johnsmith"
}
```

The `user` field does not need to correspond to a valid user object in your app. In fact, your app need not even include a user schema at all. The user Id is used to identify the token when you send push messages to users (see below), and allows you to register multiple tokens to a single user.


## Send a Push Notification

Now that you've registered devices to receive push notifications, you can send push messages.


## Sending to a specific device(s)

Each dictionary in the `tokens` list is treated as a standard device token. Since the `payload` dictionary is used to send data to multiple different devices, it's subject to the following rules:

<ol>
<li> When sending to any iOS device, any top-level key in the dictionary named "badge", "sound" or "message" will automatically be removed and sent to the iOS device as "badge", "sound" and "alert" fields (respectively) in the "aps" dictionary. All other values will be sent in the top level dictionary as custom payload key-value pairs. This means that, if you include them, any "alert" and "message" values must be JSON strings, and any "badge" value must be a JSON integer. For more detail on what iOS push payloads look like, see <a href="http://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/ApplePushService.html#//apple_ref/doc/uid/TP40008194-CH100-SW9">the "The Notification Payload" section of the iOS Developer Library</a>.
<li> iOS and Android have different payload size limits. StackMob will try to send your payload to as many devices as possible. Here are the size ranges in bytes:
<ol>
<li> 0-256 bytes - will send to both Android and iOS devices
<li> 257-1024 bytes - will send only to Android devices
<li> 1025 and above bytes - will not send to any devices
</ol>
</ol>

```bash
POST http://push.stackmob.com/notifications
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```javascript
{
  "payload": {
    "badge": 1,
    "sound": "mysound.mp3",
    "alert": "you've got mail!",
    "other1": "some data",
    "other2": "some other data"
  },
  "tokens": [
    {
      "type": "android",
      "token": "a88fqsdg8as87rgq87wrg8as8dg78zDf8a98sdf98asd8fa7sdf"
    },
    {
      "type": "ios",
      "token": "deadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"
    }
  ]
}
```

## Sending to a specific user(s)

The payload dictionary is subject to the same rules as outlined in "Pushing to Tokens" above.
Also, as mentioned above in "Registering Tokens", the users specified in the "userIds" list need not match user objects in your app, nor do you even need a user schema in your app to use this call.
Any usernames, which do not have a device token associated with them will be ignored.

```bash
POST http://push.stackmob.com/notifications
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```javascript
{
  
  "payload": {
    "badge": 1,
    "sound": "mysound.mp3",
    "alert": "you've got mail!",
    "other1": "some data",
    "other2": "some other data"
  },
  "users": ["john123", "johndoe"]
}
```

**Console**

Manually test <a
href="https://dashboard.stackmob.com/data/console?method=Send%20Push&type=push&tutorialMode=push_send">sending a push notification through
the Console</a> with the `Send Push` method.

You can send to the device token directly, but if you registered your
tokens with a user id, you can send a notification using that. Choose
via the dropdown.

The sound and badge parameters are optional. Type in a message and hit
submit.

## Broadcasting a Push Message to all devices

```bash
POST http://push.stackmob.com/notifications
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

Request Body:

```javascript
{
  "payload": {
    "badge": 1,
    "sound": "mysound.mp3",
    "alert": "you've got mail!",
    "other1": "some data",
    "other2": "some other data"
  }
}
```

**Console**

You can test sending a push notification to every user through the `Broadcast` method on
<a href="https://dashboard.stackmob.com/data/console?method=Broadcast&amp;type=push&amp;tutorialMode=push_broadcast">StackMob's web console</a>.

Broadcast will send the same notification to every registered token. (You must
<a href="https://www.stackmob.com/devcenter/docs/Setting-up-iOS-Push#a-getting_a_valid_device_token_for_your_mobile_device">register each token</a> you want to send
push messages to beforehand.)

<a href="https://dashboard.stackmob.com/data/console?method=Broadcast&amp;type=push&amp;tutorialMode=push_broadcast" class="screenshot">
    <img alt="Screenshot of Sending Broadcast" src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/StackMob_Push_Broadcast.png" />
</a>

## Removing a registered token

You can remove push tokens from the registered tokens list, so that a user will no longer receive push notifications send directly, or via broadcast command. Note, that if a push token is linked to a username (has the same username, as an entry in your 'users' table), the token will be removed automatically if that user is deleted.

```bash
DELETE http://push.stackmob.com/tokens/androidgcm/a88fqsdg8as87rgq87wrg8as8dg78zDf8a98sdf98asd8fa7sdf"
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```


**Console**

You can also manually unregister tokens by calling the `Remove Token` method in the <a href="https://dashboard.stackmob.com/data/console?method=Remove%20Token&amp;type=push">Console</a>.

<a href="https://dashboard.stackmob.com/data/console?method=Remove%20Token&amp;type=push" class="screenshot">
<img alt="Screenshot of Remove Token" src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/StackMob_Push_Remove_Token.png" />
</a>

## Checking for Expired Device Tokens

Device tokens can expire on both Android and iOS. There are 2 main reasons that tokens can expire on either platform:

1. A user turns off push notifications for your app.
2. The token has a validity time limit, and has exceeded that limit.

On Android, either an unregister intent is sent to the device (see [here](http://developer.android.com/guide/google/gcm/gcm.html#unregistering) for more details), or the Google GCM server rejects the send. 
When the former happens, you should remove the token (see previous section) from the StackMob server, and when the latter happens we automatically remove the token for you.

Apple has a push notification feedback service (APNFS) that StackMob listens to. When the feedback service informs StackMob about an expired token, we will unregister it, and add it to a list of expired tokens. You do not need to write any iOS code to ensure that expired tokens are removed.

This kind of token management is generally a server-side operation, so we do not provide methods for it from client-side SDKs.

This method has a `clear` parameter, which will clear the expired token list, if set to true. If the `clear` parameter is false, the same tokens will be returned in a subsequent call.


```bash
GET http://push.stackmob.com/expired_tokens?clear=true
```

Request Headers:

```bash
//"version" sets your REST API Version. "0" for Development. "1" and up for Production
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */
```

**Console**

You can check which tokens have been expired by calling the `Get Expired Tokens` method on
the <a href="https://dashboard.stackmob.com/data/console?method=Get%20Expired%20Tokens&amp;type=push">Console</a>.

<a href="https://dashboard.stackmob.com/data/console?method=Get%20Expired%20Tokens&amp;type=push" class="screenshot">
<img alt="Screenshot of Get Expired Tokens" src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/StackMob_Push_Get_Expired.png" />
</a>

