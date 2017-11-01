{
  "targets": [
    {
      "target_name": "addon",
      "sources": ["leven.cc" ], 
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
    
  ]
}