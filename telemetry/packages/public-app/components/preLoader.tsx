import React from 'react';
import '../app/globals.css';
import {useEffect, useState} from "react";
import GridDisplay from './gridDisplay';
var hypedLogo = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAOAAAADhCAMAAADmr0l2AAAAaVBMVEUDBwiwHyS2ICVEDhEzDA5TERU3DQ8mCQsXCg23ICQ9DxElCgtKDxAABASzICQKCAmHGRyRGh++IiV+GBsbCwohCguJGR4vDA6UGx8/DQ+aGx+oHiN8GRuCGBsdCwsxDA1WERVsFhlzFhm3iNYLAAAClUlEQVR4nO3c3W7aUBBF4ZwhCTY4xylNmtK/pH3/h0zalGKI1Erm7M4WWl+4sAQ3SyMQxGNfXAAAAAAAAAAAAAAAAAAAAAAATI2XzY3ZTQe6Tf9GvPz9PtgdvTk+PvrzdGze32RHTXVRWqtddtSUItBqhILAUu+yqyYkgU4jVASWep+dtacJ/OozQklgCZ8RagLrrc0INYElltlhO6LA+sFlhKLAUh9MCmWB7848sMRDdtorWaDLCGWBJa6z237RBZqMUBdYYpUd95MwsH50GKEw8OVdaFCoDCwOI5QG9tvsPHFg/ZQ/QmlgiUV2nziw5I9QHJg/QnFg/rtQHFj6IblQHZg+QnVgiSE38G4TWv3n3BGO25XY1ut8IQAAAPC/jKvr1ry+XHfNfy71X7L/S3FAsavmtMklCTRaAxLtqn3LrpqQBDqNULOrts7O2mMZb5bwGSHLePP4jJBlvJlcNrlYxpstHrPTXunOLpmMkGW82Tw2uVjGO4XFCJWBvcMIlYHpCwjqQIM1IJbxTmQwQpbxTtMvsgvVm07pI9Qv4515YPoHqX4Z7yq3sOtbn106PtmUvYy3UBu8zhcCAADAyE1j2T1H7p+uGnvy+nI9NP+51H+3GuIg+MGb3XRAEBg/nEaomKDVCBWBViOUTLAYfZBKAp1GqJlg9RmhKNBnMV0TaHSXWFFg2NxiVBToc22BKtBmhKrAEibvQlmgy41+ZYEuW826QJNrC3SBJiMUBnpcWyAM9BihMtDi2gJloMViujTQYYTSQIcRagMNLg/RBpa4PPPA/BGKA/OvLVAHpo9QHVgi+S6x7c8uHZ9sSt6nHJfL9d8f6/3jXy/dPZbTV7tcKQkAAAAAAAAAAAAAAAAAAABA4RkCd0bPn/PWcAAAAABJRU5ErkJggg=='

const style:React.CSSProperties = {
    backgroundColor: 'black',
    color: 'white',
    fontSize: '20px',
    justifyContent: 'center',
    alignContent: 'center',
    alignItems: 'center', // corrected property name from alignContent to alignItems
    display: 'flex',
    flexDirection: 'column', // added display property to center content horizontally
    height: '70vh', // added height to center content vertically
    width: '100vw',
    margin: 'auto',
    
}

export default function PreLoader():JSX.Element {
    //const [data, setData] = useState([])
    const [loading, setLoading] = useState<boolean>(true)
    const [processing, setProcessing] = useState<boolean>(true)
    const [processed, setProcessed] = useState<boolean>(true)

    useEffect(()=>{
      setTimeout(()=>{
        setProcessing(false)
       }, 0)
       setTimeout(()=>{
        setProcessed(false)
       },0)
    },[])

    useEffect (()=> {
      
       setTimeout(()=>{setLoading(false)
        document.documentElement.style.setProperty('--bg-color', 'white');
    }, 0)
      
     
    }, [])


  return ( 
  <div>
    {(loading)?(
    <div>
      <div style={style} className='fade-in-image'>
        <img alt ='hyped logo' 
        src={hypedLogo}
        width ='300px'
         />
         <div className ='processing'>
         {(processing)?(
          <h1>Authenticating...</h1>
        ): (processed)?(<h1>Performing Security Checks...</h1>):(<h1>Granting Access...</h1>)}
        
        </div>
      </div>
    </div>
    
    ):
    (
      <GridDisplay/>  
    )
    }
</div>
)};

