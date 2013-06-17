Saving Geolocation
==================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/geo-location.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Save Geo Location data to the StackMob datastore.

### Experience Level
Beginner

### Estimated time to complete
~5 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 5 and greater

* StackMob iOS SDK 1.3.0+

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

**Have you read through the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#CodingPractices" target="_blank">Core Data Integration Coding Practices</a>?**

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.



## Open the Base Xcode Project

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/ios-sdk/base-xcode-project-for-tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.

## Add your Public Key
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** in the method:

```obj-c,4
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}
```

## Add a property to your model 
Add a property to the **Todo** entity  with the data-type **Transformable**.

<p ><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-11.png" alt=""/></p>

## Add an NSManagedObject subclass
With the **Todo** entity selected in your data model, choose "File > New > File". In the Core Data template section, select **NSManagedObject subclass** to create class files for the entity.

<p ><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-12.png" alt=""/></p>

## Add a GeoPoint field to your schema 
You'll need to add a GeoPoint field to your **Todo** schema in the StackMob dashboard.

We've created a separate tutorial called <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-GeoPoint-Field-to-Schemas">Adding a GeoPoint Field to Schemas</a>. If this is the first time you've setup a GeoPoint field on StackMob, follow that tutorial before continuing.

## Add MapKit Framework
Under "Targets > Build Phases", add the `Mapkit.Framework`
<p ><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-08.png" alt=""/></p>

## Edit ViewController.h


Select the ViewController.h file, make it a **MKMapView Delegate** and import the **MapKit** header.


```obj-c,2,4
#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@interface ViewController : UIViewController <MKMapViewDelegate> 

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@end
```

## Create Your View 

Select the storyboard file and drag and drop a **toolbar** to your view.  Select the toolbar button and change it's label to **Save Location**
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-01.png">
<br><br>
Next, drag and drop a **map view** to your view.
<br><br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-02.png">


<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)
<br/>
<br/>
To ensure you select the button inside the toolbar, open the Document Outline (you can do so by selecting Editor > Show Document Outline).  Hold the **Control** key and click on the **Bar button item - Save Location** and drag to your **ViewController.h** file.  
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-03.png">
<br/>
<br/>
In the menu, set the Connection to **Action** and enter the name **saveLocation**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-04.png">
<br/>
<br/>
Hold the **Control** key and click on the **map view** and drag to your **ViewController.h** file.  
<br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-05.png">
<br/>
<br/>
In the menu, set the Connection to **Outlet** and enter the name **mapView**.
<br><br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-06.png">
<br/>
<br/>

## Edit ViewController.h

Your ViewController.h file should look like the following:

```obj-c,6,7
#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@interface ViewController : UIViewController <MKMapViewDelegate> 

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet MKMapView *mapView;


- (IBAction)saveLocation:(id)sender;
@end
```

## Edit ViewController.m

Add the following **highlighted code** to your ViewController.m file:

ViewController.m

```obj-c,3-10,31-36,53-74
#import "ViewController.h"
#import "AppDelegate.h"
/*
    Import the StackMob header file. //
*/
#import "StackMob.h"
/*
Import the Todo header file.
 */
#import "Todo.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize mapView = _mapView;


- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    /*
        Set the mapView delegate.
    */
    _mapView.delegate = self;
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

- (void)viewDidUnload
{
    [self setMapView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)saveLocation:(id)sender {

    /*
     Get the current latitude and longitude in an SMGeoPoint
     */
    [SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
        
        Todo *todo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
        todo.todoId = [todo assignObjectId];
        todo.title = @"My Location";
        todo.location = [NSKeyedArchiver archivedDataWithRootObject:geoPoint];
        
        /*
         Save the location to StackMob.
         */
       [self.managedObjectContext saveOnSuccess:^{
           NSLog(@"Created new object in Todo schema");
       } onFailure:^(NSError *error) {
            NSLog(@"Error creating object: %@", error);
       }];
        
    } onFailure:^(NSError *error) {
        NSLog(@"Error getting SMGeoPoint: %@", error);
    }];
}
```

## Build and Run!

Run your project. If you're working with the simulator, simulate a location in the bottom pane of Xcode.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-10.png" alt=""/>
<br/>
<br/>

Allow the app to use your current location and click the save location button. Watch the debug output for a response from StackMob.  You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a>. 


Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/geo-location.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
