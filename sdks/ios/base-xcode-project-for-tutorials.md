<h1>Base Xcode Project for Tutorials</h1>

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip" class="button">Download Full Project</a>

<br/>
<br/>

The project has the latest StackMob SDK imported as well as the basic plumbing for Core Data integration.

Let's see how we got there...

<h2>The XCode Project</h2>

The project itself is based off of the **Single View Application** template, found with File > New > Project and choosing Application > Single View Application.

<h2>Importing StackMob</h2>

<a href="https://s3.amazonaws.com/static.stackmob.com/sdks/ios/stackmob-ios-sdk-v2.0.0.zip" class="button">Download StackMob v2.0.0</a>

<br/>
<br/>

Unzip stackmob-ios-sdk-vx.x.x.zip.

Drag and drop the unzipped folder into your project's root folder:
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-StackMob.png" alt="drag and drop image of project"/></p>

Check "Copy items...", select "Create groups for.." and "Finish"
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Added-StackMob.png" alt=""/></p>

Resulting in...
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Added-StackMob-2.png" alt=""/></p>

Now under "Project > Build Settings", using the Search bar, search for `Other Linker`, double click on the `Other Linker Flags` result, and set the value to `-ObjC`
    <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-Linker-Flag.png" alt="set objc"/></p>

<h2>Add CoreData, Security, SystemConfiguration, MobileCoreServices and CoreLocation Frameworks</h2>

Under "Targets > Build Phases", add the `CoreData.Framework`.
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-CoreData.png" alt=""/></p>
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Search-for-CoreData.png" alt=""/></p>

Repeat and add the `Security.framework`, `SystemConfiguration.framework`, `MobileCoreServices.framework` and `CoreLocation.framework`.
<br/>
<br/>
<h2>Import SystemConfiguration and MobileCoreServices Frameworks</h2>

Somewhere in your project, such as your project's .pch file, import the SystemConfiguration and MobileCoreServices frameworks.
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/base-project/pchFile.png" alt=""/></p>

<br/>
<h2>Add a Data Model</h2>

File > New > File and add a **Data Model** under the iOS > Core Data section. Call it **mydatamodel**.

<h3>Add an Entity to the Data Model</h3>

All of our tutorials will use at least one Entity called Todo, which will have a parent entity called StackMob. Let's add it so we don't have to for each tutorial:

Select **mydatamodel.xcdatamodeld**, press the **Add Entity** button at the bottom, and add an Entity called **Todo**.

Add two attributes: one called **todoId** and one called **title**, both of type **String**.

Repeat this process and add a **StackMob** entity with two attributes: one called **createddate** and one called **lastmoddate**, both of type **Date**.

Finally, in the **Data Model Inspector** of the StackMob entity, check the box for **Abstract Entity**. In the **Data Model Inspector** of the Todo entity, select StackMob as the **Parent Entity**.

Your Todo entity should look like this:
	<p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/base-project/todo_entity.png" alt=""/></p>

Your StackMob entity should look like this:
    <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/base-project/stackmob_entity.png" alt=""/></p>


<h3>A Note About Property Names</h3>
StackMob field names can't actually contain uppercase letters, which our attribute **todoId** violates. However, we want to promote best practice and Apple's naming conventions prefer camelCase.

The SDK will automatically translate Core Data camel-cased property names to and from their StackMob equivalents, which in this case will appear in the StackMob Object Browser as **todo_id**. You are still welcome name your Core Data properties using the StackMob convention, or even as one word, all lowercase. 

Keep in mind that this translation only happens with the Core Data integration, so if you use the lower level Datastore API you must refer to the fields using StackMob naming conventions.  You can get the full details in the <a href="http://stackmob.github.com/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">Entity and Property Naming Conventions</a> section of the Core Data Support Specifications.  

     
<h2>Appdelegate.h</h2>

Here we add the basic Core Data pieces as well as an SMClient variable for application-wide usage:

```obj-c,2,4-5,9-12
#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@class SMClient;
@class SMCoreDataStore;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (strong, nonatomic) SMCoreDataStore *coreDataStore;
@property (strong, nonatomic) SMClient *client;

@end
```

**Important:** The reason we declare all of our variables in AppDelegate, even though they will be used in ViewController, is because AppDelegate is a special file that allows us to access variables declared in its interface file from any other file, using a shared instance of AppDelegate. We declare that instance in ViewController.m.

<h2>Appdelegate.m</h2>

Here we initialize all our variables:

```obj-c,2,5-7,12-13,17-25
#import "AppDelegate.h"
#import "StackMob.h"

@implementation AppDelegate
@synthesize client = _client;
@synthesize managedObjectModel = _managedObjectModel;
@synthesize coreDataStore = _coreDataStore;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}

- (NSManagedObjectModel *)managedObjectModel
{
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"mydatamodel" withExtension:@"momd"];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _managedObjectModel;
}

// Other methods below remain unedited

```


<h2>ViewController.h</h2>

Here we declare an NSManagedObjectContext variable:

```obj-c,5
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@end
```

<h2>ViewController.m</h2>

Here we initialize our managed object context using the managed object context declared in our AppDelegate:

```obj-c,2-3,11,13-15,23
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
     // Do any additional setup after loading the view, typically from a nib.
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

// Other methods below remain unedited
```

<h2>Read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a></h2>

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

<h2>Now we are ready to add to our project!</h2>


